#pragma once

// DeckForge automation recorder and player.
// Plain C++17, no Qt. The Mixxx glue (deckforge/automation/mixxxbridge.*)
// feeds control changes in and writes played-back values out.

#include <functional>
#include <map>
#include <set>
#include <string>

#include "deckforge/automation/automationlane.h"

namespace deckforge {

/// All lanes of a project, by control key ("[Channel1],volume").
using LaneMap = std::map<std::string, AutomationLane>;

/// How live moves are written over existing automation (DAW standard terms).
enum class AutomationMode {
    Off,   ///< nothing is recorded or played back
    Read,  ///< play back only; touching a control does not record
    Touch, ///< record while you move a control; after a short rest it returns to playback
    Latch, ///< record from the first touch until you stop; the last value holds
};

struct RecorderSettings {
    AutomationMode mode = AutomationMode::Touch;
    /// Touch mode: how long a control must rest before it hands back to
    /// playback. Default 8 beats = 2 bars of 4/4.
    double touchReleaseBeats = 8.0;
    /// Touch mode: after release, glide from your last value back to the old
    /// take over this many beats, so playback rejoins without a jump.
    double returnGlideBeats = 0.5;
    /// Optional punch range: only moves inside [punchIn, punchOut) are recorded.
    bool punchEnabled = false;
    double punchInBeat = 0.0;
    double punchOutBeat = 0.0;
    /// Only these groups record ("[Channel3]"). Empty = every group is armed.
    std::set<std::string> armedGroups;
    /// Applied to each touched lane when a pass stops. 0 disables thinning.
    double thinTolerance = 0.002;
};

/// Turns a stream of control changes into automation lanes.
/// Call controlChanged() for every change, advance() regularly with the
/// current transport position, and start()/stop() around a recording pass.
class AutomationRecorder {
  public:
    explicit AutomationRecorder(LaneMap& lanes);

    void setSettings(const RecorderSettings& settings) {
        m_settings = settings;
    }
    const RecorderSettings& settings() const {
        return m_settings;
    }

    void start(double beat);
    void stop(double beat);
    bool isRecording() const {
        return m_recording;
    }

    /// A control moved. isToggle marks buttons (play, cue, loop on/off),
    /// which are stored as steps instead of ramps.
    /// Returns true if the change was written to a lane.
    bool controlChanged(const std::string& key, double value, double beat, bool isToggle = false);

    /// Move the recorder's clock forward. Releases Touch controls that have
    /// rested long enough, holds Latch values, and closes the punch range.
    void advance(double beat);

    /// True while this control is being written live, so playback must not
    /// overwrite it.
    bool isLive(const std::string& key) const {
        return m_active.count(key) > 0;
    }
    std::size_t liveCount() const {
        return m_active.size();
    }
    /// Number of changes written since the last start().
    std::size_t eventsRecorded() const {
        return m_eventsRecorded;
    }

  private:
    struct ActiveControl {
        double lastBeat = 0.0;       ///< where the lane was last written up to
        double lastValue = 0.0;      ///< the value the user left the control at
        double lastChangeBeat = 0.0; ///< when the control last moved
    };

    bool isArmed(const std::string& key) const;
    bool inPunch(double beat) const;
    /// Hand a control back to playback at beat, keeping its last value up to
    /// there. With glideBack, ramp back to the take that existed before this
    /// pass (if there was one).
    void finish(const std::string& key, ActiveControl& active, double beat, bool glideBack);
    AutomationLane& laneFor(const std::string& key, bool isToggle);

    LaneMap& m_lanes;
    RecorderSettings m_settings;
    bool m_recording = false;
    std::map<std::string, ActiveControl> m_active;
    std::set<std::string> m_touchedThisPass;
    /// Each touched lane as it was before this pass, for gliding back.
    LaneMap m_before;
    std::size_t m_eventsRecorded = 0;
};

/// Plays lanes back by writing values to controls. Only sends a value when it
/// changed, and never fights a control the recorder holds live.
class AutomationPlayer {
  public:
    using WriteFn = std::function<void(const std::string& key, double value)>;

    explicit AutomationPlayer(const LaneMap& lanes, const AutomationRecorder* recorder = nullptr);

    /// Write every lane's value at beat through write(). Returns how many
    /// values were sent.
    std::size_t process(double beat, const WriteFn& write);

    /// Forget what was sent, e.g. after a seek, so every lane is re-sent.
    void reset() {
        m_lastSent.clear();
    }

    /// Values closer than this are not re-sent.
    static constexpr double kValueEpsilon = 1e-4;

  private:
    const LaneMap& m_lanes;
    const AutomationRecorder* m_recorder;
    std::map<std::string, double> m_lastSent;
};

} // namespace deckforge
