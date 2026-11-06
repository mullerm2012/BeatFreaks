#include "deckforge/automation/automationrecorder.h"

#include <algorithm>
#include <cmath>
#include <vector>

namespace deckforge {

AutomationRecorder::AutomationRecorder(LaneMap& lanes)
        : m_lanes(lanes) {
}

void AutomationRecorder::start(double beat) {
    (void)beat;
    m_recording = m_settings.mode == AutomationMode::Touch ||
            m_settings.mode == AutomationMode::Latch;
    m_active.clear();
    m_touchedThisPass.clear();
    m_before.clear();
    m_eventsRecorded = 0;
}

void AutomationRecorder::stop(double beat) {
    if (!m_recording) {
        return;
    }
    double end = beat;
    if (m_settings.punchEnabled) {
        end = std::min(end, m_settings.punchOutBeat);
    }
    for (auto& [key, active] : m_active) {
        finish(key,
                active,
                std::max(end, active.lastBeat),
                m_settings.mode == AutomationMode::Touch);
    }
    m_active.clear();
    m_before.clear();
    if (m_settings.thinTolerance > 0.0) {
        for (const auto& key : m_touchedThisPass) {
            auto it = m_lanes.find(key);
            if (it != m_lanes.end() && !it->second.isToggle()) {
                it->second.thin(m_settings.thinTolerance);
            }
        }
    }
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

AutomationLane& AutomationRecorder::laneFor(const std::string& key, bool isToggle) {
    auto it = m_lanes.find(key);
    if (it == m_lanes.end()) {
        it = m_lanes.emplace(key, AutomationLane(key, isToggle)).first;
    }
    return it->second;
}

bool AutomationRecorder::controlChanged(
        const std::string& key, double value, double beat, bool isToggle) {
    if (!m_recording || !isArmed(key) || !inPunch(beat)) {
        return false;
    }
    AutomationLane& lane = laneFor(key, isToggle);
    if (m_touchedThisPass.insert(key).second && !lane.isToggle()) {
        m_before.insert_or_assign(key, lane);
    }
    ++m_eventsRecorded;

    if (lane.isToggle()) {
        // Buttons are events: replace whatever the old take did at this
        // exact moment and keep the rest.
        lane.insert(beat, value);
        return true;
    }

    auto it = m_active.find(key);
    if (it == m_active.end()) {
        ActiveControl active;
        active.lastBeat = beat;
        active.lastValue = value;
        active.lastChangeBeat = beat;
        // A fresh touch: anything at exactly this beat is replaced.
        lane.eraseRangeInclusive(beat, beat);
        lane.insert(beat, value);
        m_active.emplace(key, active);
        return true;
    }

    ActiveControl& active = it->second;
    // Overwrite the old take between the previous write and now.
    lane.eraseRange(active.lastBeat, beat);
    lane.insert(beat, value);
    active.lastBeat = std::max(active.lastBeat, beat);
    active.lastValue = value;
    active.lastChangeBeat = beat;
    return true;
}

void AutomationRecorder::finish(
        const std::string& key, ActiveControl& active, double beat, bool glideBack) {
    AutomationLane& lane = laneFor(key, false);
    const auto before = m_before.find(key);
    const bool canGlide = glideBack && before != m_before.end() && !before->second.isEmpty();
    const double glideEnd = beat + std::max(0.0, m_settings.returnGlideBeats);
    // The control rested at lastValue from its last move until now.
    lane.eraseRange(active.lastBeat, canGlide ? glideEnd : beat);
    if (beat > active.lastBeat + AutomationLane::kBeatEpsilon) {
        lane.insert(beat, active.lastValue);
    }
    if (canGlide) {
        lane.insert(glideEnd, before->second.valueAt(glideEnd, active.lastValue));
    }
    active.lastBeat = beat;
}

void AutomationRecorder::advance(double beat) {
    if (!m_recording) {
        return;
    }
    const bool pastPunch = m_settings.punchEnabled && beat >= m_settings.punchOutBeat;
    std::vector<std::string> released;
    for (auto& [key, active] : m_active) {
        if (pastPunch) {
            finish(key, active, m_settings.punchOutBeat, false);
            released.push_back(key);
            continue;
        }
        if (m_settings.mode == AutomationMode::Touch &&
                beat - active.lastChangeBeat >= m_settings.touchReleaseBeats) {
            finish(key, active, active.lastChangeBeat + m_settings.touchReleaseBeats, true);
            released.push_back(key);
            continue;
        }
        if (m_settings.mode == AutomationMode::Latch) {
            // Keep clearing the old take as time passes; the held value is
            // written as one point when the pass ends.
            m_lanes.at(key).eraseRange(active.lastBeat, beat);
            m_lanes.at(key).insert(active.lastBeat, active.lastValue);
            active.lastBeat = beat;
            m_lanes.at(key).insert(beat, active.lastValue);
        }
    }
    for (const auto& key : released) {
        m_active.erase(key);
    }
}

AutomationPlayer::AutomationPlayer(const LaneMap& lanes, const AutomationRecorder* recorder)
        : m_lanes(lanes),
          m_recorder(recorder) {
}

std::size_t AutomationPlayer::process(double beat, const WriteFn& write) {
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
