#pragma once

// DeckForge automation core.
// Plain C++17, no Qt, so it can be unit-tested anywhere and used from the
// engine thread (read-only lookups do not allocate).

#include <cstddef>
#include <string>
#include <vector>

namespace deckforge {

/// How the value travels from one point to the next.
enum class CurveShape {
    Linear, ///< straight line to the next point
    Hold,   ///< keep this value until the next point (buttons, toggles)
};

/// One automation point. Positions are in beats from the project start, so a
/// lane stays locked to the music when the project tempo changes.
struct AutomationPoint {
    double beat = 0.0;
    double value = 0.0;
    CurveShape shape = CurveShape::Linear;
};

/// A list of automation points for one control, for example
/// "[Channel1],volume". Points are always kept sorted by beat.
class AutomationLane {
  public:
    /// Two points closer than this (in beats) are treated as the same position.
    static constexpr double kBeatEpsilon = 1e-6;

    AutomationLane() = default;
    explicit AutomationLane(std::string controlKey, bool isToggle = false);

    const std::string& controlKey() const {
        return m_controlKey;
    }
    /// The Mixxx group part of the key, e.g. "[Channel1]".
    std::string group() const;

    /// Toggles (play, cue, mute, loop on/off) use Hold curves by default.
    bool isToggle() const {
        return m_isToggle;
    }

    bool isEmpty() const {
        return m_points.empty();
    }
    std::size_t size() const {
        return m_points.size();
    }
    const std::vector<AutomationPoint>& points() const {
        return m_points;
    }

    /// Insert a point, keeping the lane sorted. A point at the same beat is
    /// replaced. Returns the index of the point.
    std::size_t insert(const AutomationPoint& point);
    /// Convenience overload; the shape follows isToggle().
    std::size_t insert(double beat, double value);

    /// Remove every point with fromBeat < beat <= toBeat (half-open on the
    /// left so a point written exactly at fromBeat survives).
    std::size_t eraseRange(double fromBeat, double toBeat);
    /// Remove every point with fromBeat <= beat <= toBeat.
    std::size_t eraseRangeInclusive(double fromBeat, double toBeat);
    void clear() {
        m_points.clear();
    }

    /// The automated value at a beat. Before the first point the first value
    /// holds; after the last point the last value holds. An empty lane
    /// returns fallback. Does not allocate.
    double valueAt(double beat, double fallback) const;

    /// Move every point at or after fromBeat by deltaBeats (used when a clip
    /// is dragged on the timeline). Points pushed before beat 0 are dropped.
    void shift(double fromBeat, double deltaBeats);

    /// Move only the points with fromBeat <= beat <= toBeat by deltaBeats,
    /// replacing whatever sits where they land (used when one clip moves and
    /// its moves must travel with it).
    void shiftRange(double fromBeat, double toBeat, double deltaBeats);

    /// Reduce a dense live recording to the points that matter, using the
    /// Ramer-Douglas-Peucker algorithm. Points whose removal changes the curve
    /// by no more than tolerance (in value units) are dropped. Hold points
    /// and the first and last point are always kept.
    std::size_t thin(double tolerance);
    /// Same, but only points with fromBeat <= beat <= toBeat may be dropped,
    /// so a recording pass never reshapes the take around it.
    std::size_t thin(double fromBeat, double toBeat, double tolerance);

    bool operator==(const AutomationLane& other) const;

  private:
    /// Index of the first point with beat >= target.
    std::size_t lowerBound(double beat) const;

    std::string m_controlKey;
    bool m_isToggle = false;
    std::vector<AutomationPoint> m_points;
};

} // namespace deckforge
