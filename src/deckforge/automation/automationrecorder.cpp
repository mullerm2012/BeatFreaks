#include "deckforge/automation/automationrecorder.h"

#include <algorithm>
#include <cmath>
#include <vector>

namespace deckforge {

namespace {
constexpr double kEps = AutomationLane::kBeatEpsilon;
} // namespace

AutomationRecorder::AutomationRecorder(LaneMap& lanes)
        : m_lanes(lanes) {
}

void AutomationRecorder::start(double beat) {
    if (m_recording) {
        stop(beat);
    }
    m_recording = m_settings.mode == AutomationMode::Touch ||
            m_settings.mode == AutomationMode::Latch;
    m_active.clear();
    m_latchedToggles.clear();
    m_before.clear();
    m_touchBase.clear();
    m_written.clear();
    m_eventsRecorded = 0;
}

void AutomationRecorder::stop(double beat) {
    if (!m_recording) {
        return;
    }
    // Let Touch controls that rested long enough release at their own time.
    advance(beat);
    const double end = clampToPunchOut(beat);
    for (auto& [key, active] : m_active) {
        handback(key,
                active,
                end,
                m_settings.mode == AutomationMode::Touch ? Handback::Glide : Handback::Cut);
    }
    m_active.clear();
    std::vector<std::string> toggles;
    for (const auto& [key, from] : m_latchedToggles) {
        toggles.push_back(key);
    }
    for (const auto& key : toggles) {
        handbackToggle(key, end);
    }
    if (m_settings.thinTolerance > 0.0) {
        for (const auto& [key, range] : m_written) {
            auto it = m_lanes.find(key);
            if (it != m_lanes.end() && !it->second.isToggle()) {
                // Keep the join points at the edges: thin strictly inside.
                it->second.thin(range.first + kJoinBeats,
                        range.second - kJoinBeats,
                        m_settings.thinTolerance);
            }
        }
    }
    m_before.clear();
    m_touchBase.clear();
    m_recording = false;
}

bool AutomationRecorder::isArmed(const std::string& key) const {
    if (m_settings.armedGroups.empty()) {
        return true;
    }
    const auto comma = key.find(',');
    const std::string group = comma == std::string::npos ? key : key.substr(0, comma);
    return m_settings.armedGroups.count(group) > 0;
}

bool AutomationRecorder::inPunch(double beat) const {
    if (!m_settings.punchEnabled) {
        return true;
    }
    return beat >= m_settings.punchInBeat && beat < m_settings.punchOutBeat;
}

double AutomationRecorder::clampToPunchOut(double beat) const {
    return m_settings.punchEnabled ? std::min(beat, m_settings.punchOutBeat) : beat;
}

AutomationLane& AutomationRecorder::laneFor(const std::string& key, bool isToggle) {
    auto it = m_lanes.find(key);
    if (it == m_lanes.end()) {
        it = m_lanes.emplace(key, AutomationLane(key, isToggle)).first;
    } else if (it->second.isEmpty() && it->second.isToggle() != isToggle) {
        // An empty lane takes the caller's word for what kind of control it is.
        it->second = AutomationLane(key, isToggle);
    }
    if (m_before.count(key) == 0) {
        m_before.emplace(key, it->second);
    }
    return it->second;
}

const AutomationLane* AutomationRecorder::before(const std::string& key) const {
    const auto it = m_before.find(key);
    return (it == m_before.end() || it->second.isEmpty()) ? nullptr : &it->second;
}

void AutomationRecorder::noteWritten(const std::string& key, double fromBeat, double toBeat) {
    auto it = m_written.find(key);
    if (it == m_written.end()) {
        m_written.emplace(key, std::make_pair(fromBeat, toBeat));
    } else {
        it->second.first = std::min(it->second.first, fromBeat);
        it->second.second = std::max(it->second.second, toBeat);
    }
}

bool AutomationRecorder::controlChanged(
        const std::string& key, double value, double beat, bool isToggle) {
    if (!m_recording || !isArmed(key) || !inPunch(beat)) {
        return false;
    }
    AutomationLane& lane = laneFor(key, isToggle);
    ++m_eventsRecorded;

    if (isToggle || lane.isToggle()) {
        recordToggle(key, value, beat);
        return true;
    }

    auto it = m_active.find(key);
    if (it != m_active.end() && beat < it->second.lastBeat - kEps) {
        // Looped or seeked back: close what was recorded so far, then start
        // over at the new position.
        handback(key, it->second, it->second.lastBeat + kJoinBeats, Handback::Cut);
        m_active.erase(it);
        it = m_active.end();
    }
    if (it == m_active.end()) {
        beginTouch(key, value, beat);
        return true;
    }

    ActiveControl& active = it->second;
    // Overwrite the old take between the previous write and now.
    lane.eraseRange(active.lastBeat, beat);
    lane.insert(beat, value);
    noteWritten(key, active.lastBeat, beat);
    active.lastBeat = beat;
    active.lastValue = value;
    active.lastChangeBeat = beat;
    return true;
}

void AutomationRecorder::beginTouch(const std::string& key, double value, double beat) {
    AutomationLane& lane = m_lanes.at(key);
    m_touchBase.insert_or_assign(key, lane);
    // Pin the curve just before the touch so the old take up to here keeps
    // its exact shape instead of sloping toward the new value.
    const double joinBeat = beat - kJoinBeats;
    if (!lane.isEmpty() && joinBeat >= 0.0) {
        const double joinValue = lane.valueAt(joinBeat, value);
        lane.eraseRange(joinBeat, beat);
        lane.insert(joinBeat, joinValue);
    } else {
        lane.eraseRangeInclusive(beat, beat);
    }
    lane.insert(beat, value);
    noteWritten(key, std::max(0.0, joinBeat), beat);
    ActiveControl active;
    active.lastBeat = beat;
    active.lastValue = value;
    active.lastChangeBeat = beat;
    m_active.insert_or_assign(key, active);
}

void AutomationRecorder::handback(
        const std::string& key, ActiveControl& active, double beat, Handback how) {
    AutomationLane& lane = m_lanes.at(key);
    // Never reach back into what this pass already wrote.
    beat = std::max(beat, active.lastBeat);

    double holdEnd = beat;  // the user's value lasts until here
    double joinBeat = beat; // the old take resumes from here
    if (how == Handback::Glide) {
        joinBeat = clampToPunchOut(beat + std::max(0.0, m_settings.returnGlideBeats));
    }
    if (joinBeat <= holdEnd + kEps) {
        // No room to glide (cut, or glide clipped at punch-out): step back
        // to the old take right at the boundary.
        holdEnd = std::max(active.lastBeat, beat - kJoinBeats);
        joinBeat = std::max(beat, holdEnd + kJoinBeats);
    }

    const auto base = m_touchBase.find(key);
    const AutomationLane* old =
            (base == m_touchBase.end() || base->second.isEmpty()) ? nullptr : &base->second;
    if (old == nullptr) {
        // Nothing to return to: the last value simply holds.
        if (beat > active.lastBeat + kEps) {
            lane.eraseRange(active.lastBeat, beat);
            lane.insert(beat, active.lastValue);
            noteWritten(key, active.lastBeat, beat);
        }
        active.lastBeat = beat;
        return;
    }
    const double oldValue = old->valueAt(joinBeat, active.lastValue);
    lane.eraseRange(active.lastBeat, joinBeat);
    if (holdEnd > active.lastBeat + kEps) {
        lane.insert(holdEnd, active.lastValue);
    }
    lane.insert(joinBeat, oldValue);
    noteWritten(key, active.lastBeat, joinBeat);
    active.lastBeat = joinBeat;
}

void AutomationRecorder::recordToggle(const std::string& key, double value, double beat) {
    AutomationLane& lane = m_lanes.at(key);
    if (m_settings.mode == AutomationMode::Latch) {
        auto it = m_latchedToggles.find(key);
        if (it != m_latchedToggles.end() && beat > it->second) {
            // Clear the old take's presses between the last write and now.
            lane.eraseRange(it->second, beat);
        }
        m_latchedToggles[key] = beat;
    }
    lane.insert(AutomationPoint{beat, value, CurveShape::Hold});
    noteWritten(key, beat, beat);
}

void AutomationRecorder::handbackToggle(const std::string& key, double beat) {
    auto it = m_latchedToggles.find(key);
    if (it == m_latchedToggles.end()) {
        return;
    }
    AutomationLane& lane = m_lanes.at(key);
    const double from = it->second;
    m_latchedToggles.erase(it);
    if (beat <= from + kEps) {
        return;
    }
    lane.eraseRange(from, beat);
    // After the pass the old take's state comes back, if it differs.
    if (const AutomationLane* old = before(key)) {
        const double oldValue = old->valueAt(beat, 0.0);
        if (std::fabs(lane.valueAt(beat, oldValue) - oldValue) > 1e-9) {
            lane.insert(AutomationPoint{beat, oldValue, CurveShape::Hold});
        }
    }
    noteWritten(key, from, beat);
}

void AutomationRecorder::advance(double beat) {
    if (!m_recording) {
        return;
    }
    std::vector<std::string> released;
    for (auto& [key, active] : m_active) {
        if (beat < active.lastBeat - kEps) {
            // Looped or seeked back: close the take; touch again to record.
            handback(key, active, active.lastBeat + kJoinBeats, Handback::Cut);
            released.push_back(key);
            continue;
        }
        const double releaseAt = active.lastChangeBeat + m_settings.touchReleaseBeats;
        const bool releaseBeforePunchOut =
                !m_settings.punchEnabled || releaseAt < m_settings.punchOutBeat;
        if (m_settings.mode == AutomationMode::Touch && beat >= releaseAt &&
                releaseBeforePunchOut) {
            handback(key, active, releaseAt, Handback::Glide);
            released.push_back(key);
            continue;
        }
        if (m_settings.punchEnabled && beat >= m_settings.punchOutBeat) {
            handback(key, active, m_settings.punchOutBeat, Handback::Cut);
            released.push_back(key);
            continue;
        }
        if (m_settings.mode == AutomationMode::Latch && inPunch(beat) &&
                beat > active.lastBeat + kEps) {
            // Keep the latched value, overwriting the old take as time passes.
            AutomationLane& lane = m_lanes.at(key);
            lane.eraseRange(active.lastBeat, beat);
            lane.insert(beat, active.lastValue);
            noteWritten(key, active.lastBeat, beat);
            active.lastBeat = beat;
        }
    }
    for (const auto& key : released) {
        m_active.erase(key);
    }

    std::vector<std::string> togglesDone;
    for (auto& [key, from] : m_latchedToggles) {
        if (beat < from - kEps || (m_settings.punchEnabled && beat >= m_settings.punchOutBeat)) {
            togglesDone.push_back(key);
        } else if (beat > from + kEps) {
            m_lanes.at(key).eraseRange(from, beat);
            noteWritten(key, from, beat);
            from = beat;
        }
    }
    for (const auto& key : togglesDone) {
        const double from = m_latchedToggles.at(key);
        handbackToggle(key, beat < from ? from : clampToPunchOut(beat));
    }
}

AutomationPlayer::AutomationPlayer(const LaneMap& lanes, const AutomationRecorder* recorder)
        : m_lanes(lanes),
          m_recorder(recorder) {
}

std::size_t AutomationPlayer::process(double beat, const WriteFn& write) {
    if (m_recorder != nullptr && m_recorder->settings().mode == AutomationMode::Off) {
        return 0;
    }
    std::size_t sent = 0;
    for (const auto& [key, lane] : m_lanes) {
        if (lane.isEmpty()) {
            continue;
        }
        if (m_recorder != nullptr && m_recorder->isLive(key)) {
            // The DJ has this control; forget our last value so playback
            // resumes cleanly when it is released.
            m_lastSent.erase(key);
            continue;
        }
        const double value = lane.valueAt(beat, 0.0);
        auto it = m_lastSent.find(key);
        if (it != m_lastSent.end() && std::fabs(it->second - value) < kValueEpsilon) {
            continue;
        }
        write(key, value);
        m_lastSent[key] = value;
        ++sent;
    }
    return sent;
}

} // namespace deckforge
