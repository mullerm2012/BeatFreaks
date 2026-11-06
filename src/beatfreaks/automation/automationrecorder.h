#pragma once

// BeatFreaks automation recorder and player.
// Plain C++17, no Qt. The Mixxx glue feeds control changes in and writes
// played-back values out.
//
// Rule the recorder keeps: a pass only changes the lane where you actually
// recorded. Outside that region (before a touch, after a release, outside the
// punch range) the curve is exactly what it was before the pass.

#include <functional>
#include <map>
#include <set>
#include <string>
#include <utility>

#include "beatfreaks/automation/automationlane.h"

namespace beatfreaks {

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
    /// Applied to the recorded part of each lane when a pass stops. 0 disables.
    double thinTolerance = 0.002;
};

/// Turns a stream of control changes into automation lanes.
/// Call controlChanged() for every change, advance() regularly with the
/// current transport position, and start()/stop() around a recording pass.
/// Jumping backwards (a loop or a seek) is allowed at any time.
class AutomationRecorder {
  public:
    /// The gap used to join new and old automation without a sloped seam.
    static constexpr double kJoinBeats = 1.0 / 1024.0;

    explicit AutomationRecorder(LaneMap& lanes);

    void setSettings(const RecorderSettings& settings) {
        m_settings = settings;
    }
    const RecorderSettings& settings() const {
        return m_settings;
    }

    /// Starts a pass. If a pass is running it is stopped first, at beat.
    void start(double beat);
    void stop(double beat);
    bool isRecording() const {
        return m_recording;
    }

    /// A control moved. isToggle marks buttons (play, cue, loop on/off),
    /// which are stored as steps instead of ramps.
    /// Returns true if the change was written to a lane.
    bool controlChanged(const std::string& key, double value, double beat, bool isToggle = false);

    /// Move the recorder's clock. Releases Touch controls that have rested
    /// long enough, holds Latch values, and closes the punch range.
    void advance(double beat);

    /// True while this control is being written live, so playback must not
    /// overwrite it.
    bool isLive(const std::string& key) const {
        return m_active.count(key) > 0 || m_latchedToggles.count(key) > 0;
    }
    std::size_t liveCount() const {
        return m_active.size() + m_latchedToggles.size();
    }
    /// Number of changes written since the last start().
    std::size_t eventsRecorded() const {
        return m_eventsRecorded;
    }

  private:
    struct ActiveControl {
        double lastBeat = 0.0;       ///< the lane holds this pass's take up to here
        double lastValue = 0.0;      ///< the value the user left the control at
        double lastChangeBeat = 0.0; ///< when the control last moved
    };
    enum class Handback {
        Glide, ///< Touch release: hold, then glide back to the old take
        Cut,   ///< punch-out, Latch stop, loop: old take resumes exactly at the beat
    };

    bool isArmed(const std::string& key) const;
    bool inPunch(double beat) const;
    double clampToPunchOut(double beat) const;
    AutomationLane& laneFor(const std::string& key, bool isToggle);
    /// Snapshot of the lane before this pass first wrote to it.
    const AutomationLane* before(const std::string& key) const;
    void noteWritten(const std::string& key, double fromBeat, double toBeat);

    void beginTouch(const std::string& key, double value, double beat);
    /// Hand a control back to playback at beat.
    void handback(const std::string& key, ActiveControl& active, double beat, Handback how);
    void recordToggle(const std::string& key, double value, double beat);
    void handbackToggle(const std::string& key, double beat);

    LaneMap& m_lanes;
    RecorderSettings m_settings;
    bool m_recording = false;
    std::map<std::string, ActiveControl> m_active;
    /// Latch mode buttons: key -> beat up to which the old take was cleared.
    std::map<std::string, double> m_latchedToggles;
    LaneMap m_before;
    /// Each live control's lane as it was when this touch began: the take a
    /// release returns to (after a loop, that is the first lap, not the
    /// take from before the pass).
    LaneMap m_touchBase;
    /// Per lane, the beat range this pass wrote (for thinning).
    std::map<std::string, std::pair<double, double>> m_written;
    std::size_t m_eventsRecorded = 0;
};

/// Plays lanes back by writing values to controls. Only sends a value when it
/// changed, and never fights a control the recorder holds live.
class AutomationPlayer {
  public:
    using WriteFn = std::function<void(const std::string& key, double value)>;

    explicit AutomationPlayer(const LaneMap& lanes, const AutomationRecorder* recorder = nullptr);

    /// Write every lane's value at beat through write(). Returns how many
    /// values were sent. Sends nothing when the recorder's mode is Off.
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

} // namespace beatfreaks
