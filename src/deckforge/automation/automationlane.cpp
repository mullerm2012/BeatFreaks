#include "deckforge/automation/automationlane.h"

#include <algorithm>
#include <cmath>
#include <utility>

namespace deckforge {

AutomationLane::AutomationLane(std::string controlKey, bool isToggle)
        : m_controlKey(std::move(controlKey)),
          m_isToggle(isToggle) {
}

std::string AutomationLane::group() const {
    const auto comma = m_controlKey.find(',');
    return comma == std::string::npos ? m_controlKey : m_controlKey.substr(0, comma);
}

std::size_t AutomationLane::lowerBound(double beat) const {
    const auto it = std::lower_bound(m_points.begin(),
            m_points.end(),
            beat - kBeatEpsilon,
            [](const AutomationPoint& p, double b) { return p.beat < b; });
    return static_cast<std::size_t>(it - m_points.begin());
}

std::size_t AutomationLane::insert(const AutomationPoint& point) {
    const std::size_t index = lowerBound(point.beat);
    if (index < m_points.size() &&
            std::fabs(m_points[index].beat - point.beat) <= kBeatEpsilon) {
        m_points[index] = point;
        return index;
    }
    m_points.insert(m_points.begin() + static_cast<std::ptrdiff_t>(index), point);
    return index;
}

std::size_t AutomationLane::insert(double beat, double value) {
    return insert(AutomationPoint{beat,
            value,
            m_isToggle ? CurveShape::Hold : CurveShape::Linear});
}

std::size_t AutomationLane::eraseRange(double fromBeat, double toBeat) {
    if (toBeat < fromBeat) {
        return 0;
    }
    const auto first = std::upper_bound(m_points.begin(),
            m_points.end(),
            fromBeat + kBeatEpsilon,
            [](double b, const AutomationPoint& p) { return b < p.beat; });
    const auto last = std::upper_bound(first,
            m_points.end(),
            toBeat + kBeatEpsilon,
            [](double b, const AutomationPoint& p) { return b < p.beat; });
    const auto removed = static_cast<std::size_t>(last - first);
    m_points.erase(first, last);
    return removed;
}

std::size_t AutomationLane::eraseRangeInclusive(double fromBeat, double toBeat) {
    if (toBeat < fromBeat) {
        return 0;
    }
    const auto first = m_points.begin() + static_cast<std::ptrdiff_t>(lowerBound(fromBeat));
    const auto last = std::upper_bound(first,
            m_points.end(),
            toBeat + kBeatEpsilon,
            [](double b, const AutomationPoint& p) { return b < p.beat; });
    const auto removed = static_cast<std::size_t>(last - first);
    m_points.erase(first, last);
    return removed;
}

double AutomationLane::valueAt(double beat, double fallback) const {
    if (m_points.empty()) {
        return fallback;
    }
    if (beat <= m_points.front().beat) {
        return m_points.front().value;
    }
    if (beat >= m_points.back().beat) {
        return m_points.back().value;
    }
    // First point strictly after beat; it exists because of the checks above.
    const auto next = std::upper_bound(m_points.begin(),
            m_points.end(),
            beat,
            [](double b, const AutomationPoint& p) { return b < p.beat; });
    const AutomationPoint& b = *next;
    const AutomationPoint& a = *(next - 1);
    if (a.shape == CurveShape::Hold || b.beat - a.beat <= kBeatEpsilon) {
        return a.value;
    }
    const double t = (beat - a.beat) / (b.beat - a.beat);
    return a.value + (b.value - a.value) * t;
}

void AutomationLane::shift(double fromBeat, double deltaBeats) {
    if (deltaBeats == 0.0) {
        return;
    }
    std::vector<AutomationPoint> moved;
    std::vector<AutomationPoint> kept;
    for (const auto& p : m_points) {
        if (p.beat >= fromBeat - kBeatEpsilon) {
            AutomationPoint q = p;
            q.beat += deltaBeats;
            if (q.beat >= -kBeatEpsilon) {
                q.beat = std::max(0.0, q.beat);
                moved.push_back(q);
            }
        } else {
            kept.push_back(p);
        }
    }
    m_points = std::move(kept);
    for (const auto& p : moved) {
        insert(p); // moved points win over kept points at the same beat
    }
}

void AutomationLane::shiftRange(double fromBeat, double toBeat, double deltaBeats) {
    if (deltaBeats == 0.0 || toBeat < fromBeat) {
        return;
    }
    std::vector<AutomationPoint> moving;
    for (const auto& p : m_points) {
        if (p.beat >= fromBeat - kBeatEpsilon && p.beat <= toBeat + kBeatEpsilon) {
            moving.push_back(p);
        }
    }
    if (moving.empty()) {
        return;
    }
    eraseRangeInclusive(fromBeat, toBeat);
    // Clear the landing zone so the moved block replaces what was there.
    eraseRangeInclusive(std::max(0.0, fromBeat + deltaBeats), toBeat + deltaBeats);
    for (auto p : moving) {
        p.beat += deltaBeats;
        if (p.beat >= -kBeatEpsilon) {
            p.beat = std::max(0.0, p.beat);
            insert(p);
        }
    }
}

namespace {

double perpendicularDistance(const AutomationPoint& p,
        const AutomationPoint& a,
        const AutomationPoint& b) {
    // Vertical distance from p to the straight line a-b. Using the vertical
    // (value) distance keeps the tolerance in the control's own units.
    if (b.beat - a.beat <= AutomationLane::kBeatEpsilon) {
        return std::fabs(p.value - a.value);
    }
    const double t = (p.beat - a.beat) / (b.beat - a.beat);
    const double onLine = a.value + (b.value - a.value) * t;
    return std::fabs(p.value - onLine);
}

void simplify(const std::vector<AutomationPoint>& pts,
        std::size_t first,
        std::size_t last,
        double tolerance,
        std::vector<bool>& keep) {
    if (last <= first + 1) {
        return;
    }
    double maxDistance = -1.0;
    std::size_t maxIndex = first;
    for (std::size_t i = first + 1; i < last; ++i) {
        const double d = perpendicularDistance(pts[i], pts[first], pts[last]);
        if (d > maxDistance) {
            maxDistance = d;
            maxIndex = i;
        }
    }
    if (maxDistance > tolerance) {
        keep[maxIndex] = true;
        simplify(pts, first, maxIndex, tolerance, keep);
        simplify(pts, maxIndex, last, tolerance, keep);
    }
}

} // namespace

std::size_t AutomationLane::thin(double tolerance) {
    if (m_points.size() < 3) {
        return 0;
    }
    std::vector<bool> keep(m_points.size(), false);
    keep.front() = true;
    keep.back() = true;
    // Hold points (and the point right after one) mark steps; never drop them.
    for (std::size_t i = 0; i < m_points.size(); ++i) {
        if (m_points[i].shape == CurveShape::Hold) {
            keep[i] = true;
            if (i + 1 < m_points.size()) {
                keep[i + 1] = true;
            }
        }
    }
    // Simplify each run between kept anchors separately.
    std::size_t anchor = 0;
    for (std::size_t i = 1; i < m_points.size(); ++i) {
        if (keep[i]) {
            simplify(m_points, anchor, i, tolerance, keep);
            anchor = i;
        }
    }
    std::vector<AutomationPoint> result;
    result.reserve(m_points.size());
    for (std::size_t i = 0; i < m_points.size(); ++i) {
        if (keep[i]) {
            result.push_back(m_points[i]);
        }
    }
    const std::size_t removed = m_points.size() - result.size();
    m_points = std::move(result);
    return removed;
}

bool AutomationLane::operator==(const AutomationLane& other) const {
    if (m_controlKey != other.m_controlKey || m_isToggle != other.m_isToggle ||
            m_points.size() != other.m_points.size()) {
        return false;
    }
    for (std::size_t i = 0; i < m_points.size(); ++i) {
        const auto& a = m_points[i];
        const auto& b = other.m_points[i];
        if (std::fabs(a.beat - b.beat) > kBeatEpsilon ||
                std::fabs(a.value - b.value) > 1e-9 || a.shape != b.shape) {
            return false;
        }
    }
    return true;
}

} // namespace deckforge
