// Edge cases found in review: loops, seeks, punch boundaries, latch buttons,
// thinning scope and clip moves at the timeline start.
#include <gtest/gtest.h>
#include <QJsonArray>
#include <QString>
#include "beatfreaks/automation/automationrecorder.h"
#include "beatfreaks/project/bfproject.h"
using namespace beatfreaks;
namespace {
const std::string kVol = "[Channel1],volume";
const std::string kPlay = "[Channel1],play";
AutomationLane ramp(const std::string& k, double b0, double v0, double b1, double v1) {
    AutomationLane l(k); l.insert(b0, v0); l.insert(b1, v1); return l;
}
RecorderSettings noThin(AutomationMode m = AutomationMode::Touch) {
    RecorderSettings s; s.mode = m; s.thinTolerance = 0.0; return s;
}
}

// 1. Punch-out: automation AFTER punch-out changes.
TEST(AutomationEdgeCaseTest, PunchOutChangesCurveAfterPunch) {
    LaneMap lanes; lanes.emplace(kVol, ramp(kVol, 0, 0.0, 32, 1.0));
    AutomationRecorder rec(lanes);
    auto s = noThin(); s.punchEnabled = true; s.punchInBeat = 8; s.punchOutBeat = 16;
    rec.setSettings(s); rec.start(0);
    rec.advance(10); rec.controlChanged(kVol, 0.9, 10);
    rec.advance(16);
    rec.stop(40);
    EXPECT_NEAR(20.0 / 32, lanes.at(kVol).valueAt(20, -1), 1e-6); // old take outside punch
}

// 2. Fresh touch inside punch changes curve BEFORE punch-in.
TEST(AutomationEdgeCaseTest, PunchInChangesCurveBeforePunch) {
    LaneMap lanes; lanes.emplace(kVol, ramp(kVol, 0, 0.0, 32, 1.0));
    AutomationRecorder rec(lanes);
    auto s = noThin(); s.punchEnabled = true; s.punchInBeat = 8; s.punchOutBeat = 16;
    rec.setSettings(s); rec.start(0);
    rec.advance(9); rec.controlChanged(kVol, 0.9, 9);
    rec.advance(16); rec.stop(16);
    EXPECT_NEAR(7.0 / 32, lanes.at(kVol).valueAt(7, -1), 1e-6);
}

// 3. Touch release glide crosses punch-out and erases old take past punch-out.
TEST(AutomationEdgeCaseTest, TouchGlideErasesPastPunchOut) {
    LaneMap lanes; AutomationLane l(kVol);
    l.insert(0, 0.0); l.insert(16.25, 1.0); l.insert(16.5, 0.0); l.insert(64, 0.0);
    lanes.emplace(kVol, l);
    AutomationRecorder rec(lanes);
    auto s = noThin(); s.punchEnabled = true; s.punchInBeat = 0; s.punchOutBeat = 16;
    s.touchReleaseBeats = 8; s.returnGlideBeats = 1.0;
    rec.setSettings(s); rec.start(0);
    rec.advance(7.5); rec.controlChanged(kVol, 0.5, 7.5); // release at 15.5, glide to 16.5
    rec.advance(15.5); rec.advance(17); rec.stop(20);
    EXPECT_NEAR(1.0, lanes.at(kVol).valueAt(16.25, -1), 1e-6); // old point past punch-out
}

// 4. Loop/backward seek during a Touch pass: glide point is dropped into the
// middle of this pass's own recording.
TEST(AutomationEdgeCaseTest, BackwardSeekTouchGlideInsideRecording) {
    LaneMap lanes; AutomationLane l(kVol); l.insert(0, 0.5); l.insert(64, 0.5);
    lanes.emplace(kVol, l);
    AutomationRecorder rec(lanes);
    auto s = noThin(); rec.setSettings(s); rec.start(0);
    for (int b = 0; b <= 20; ++b) { rec.advance(b); rec.controlChanged(kVol, 1.0, b); }
    rec.advance(2); rec.controlChanged(kVol, 1.0, 2); // loop back, same value
    for (double b = 2; b <= 12; b += 0.25) rec.advance(b);
    const auto& lane = lanes.at(kVol);
    EXPECT_NEAR(1.0, lane.valueAt(10.5, -1), 1e-6); // DJ held 1.0 the whole time
}

// 5. Latch: a backward advance (loop) writes outside the punch range.
TEST(AutomationEdgeCaseTest, LatchBackwardAdvanceWritesOutsidePunch) {
    LaneMap lanes; lanes.emplace(kVol, ramp(kVol, 0, 0.2, 64, 0.2));
    AutomationRecorder rec(lanes);
    auto s = noThin(AutomationMode::Latch); s.punchEnabled = true;
    s.punchInBeat = 8; s.punchOutBeat = 32;
    rec.setSettings(s); rec.start(0);
    rec.advance(10); rec.controlChanged(kVol, 0.9, 10);
    rec.advance(20); rec.advance(2); rec.advance(4);
    rec.stop(4);
    EXPECT_NEAR(0.2, lanes.at(kVol).valueAt(3, -1), 1e-6);
}

// 6. Toggles in Latch: old take's later events survive the latched pass.
TEST(AutomationEdgeCaseTest, LatchToggleOldEventsSurvive) {
    LaneMap lanes; AutomationLane l(kPlay, true); l.insert(0, 1.0); l.insert(8, 0.0); l.insert(12, 1.0);
    lanes.emplace(kPlay, l);
    AutomationRecorder rec(lanes); rec.setSettings(noThin(AutomationMode::Latch));
    rec.start(0); rec.controlChanged(kPlay, 0.0, 4, true);
    for (double b = 4; b <= 20; b += 1) rec.advance(b);
    rec.stop(20);
    EXPECT_DOUBLE_EQ(0.0, lanes.at(kPlay).valueAt(14, -1)); // latched 0 until stop
}

// 7. Thinning on stop modifies the old take outside the pass.
TEST(AutomationEdgeCaseTest, ThinningTouchesUntouchedRegion) {
    LaneMap lanes; AutomationLane l(kVol);
    l.insert(0, 0.0); l.insert(4, 0.0015); l.insert(8, 0.0); l.insert(100, 0.0);
    lanes.emplace(kVol, l);
    AutomationRecorder rec(lanes); RecorderSettings s; rec.setSettings(s);
    rec.start(0); rec.advance(50); rec.controlChanged(kVol, 0.0, 50); rec.stop(52);
    EXPECT_NEAR(0.0015, lanes.at(kVol).valueAt(4, -1), 1e-9);
}


// 9. moveClip clamped at 0: clip moves less than its automation.
TEST(AutomationEdgeCaseTest, MoveClipClampDesyncsAutomation) {
    BfProject p; Clip c; c.id = QStringLiteral("c"); c.channel = 0; c.startBeat = 2; c.lengthBeats = 8;
    p.clips.append(c);
    AutomationLane l(kVol); l.insert(2, 0.1); l.insert(6, 0.6); l.insert(10, 1.0);
    p.lanes.emplace(kVol, l);
    p.moveClip(QStringLiteral("c"), -4);
    EXPECT_DOUBLE_EQ(0.0, p.clips[0].startBeat);
    // Automation should keep its offset relative to the clip start.
    EXPECT_NEAR(0.1, p.lanes.at(kVol).valueAt(0, -1), 1e-9);
    EXPECT_NEAR(0.6, p.lanes.at(kVol).valueAt(4, -1), 1e-9);
}


// 11. JSON load: unsorted + duplicate points.
TEST(AutomationEdgeCaseTest, JsonUnsortedDuplicates) {
    QJsonObject o{{"format", "beatfreaks-project"}, {"version", 1},
        {"lanes", QJsonArray{QJsonObject{{"control", "[Channel1],volume"},
            {"points", QJsonArray{QJsonArray{8, 0.8}, QJsonArray{0, 0.0}, QJsonArray{8, 0.3},
                                  QJsonArray{4, 0.5}}}}}}};
    auto p = BfProject::fromJson(o);
    ASSERT_TRUE(p);
    const auto& l = p->lanes.at(kVol);
    ASSERT_EQ(3u, l.size());
    EXPECT_DOUBLE_EQ(0.3, l.points()[2].value);
}

// 12. Toggle sent with isToggle=true to an existing non-toggle lane (e.g.
// loaded with "toggle" missing) is recorded as a fader and goes live.
TEST(AutomationEdgeCaseTest, ToggleFlagIgnoredForExistingLane) {
    LaneMap lanes; lanes.emplace(kPlay, AutomationLane(kPlay));
    AutomationRecorder rec(lanes); rec.setSettings(noThin());
    rec.start(0); rec.controlChanged(kPlay, 1.0, 4, true);
    EXPECT_FALSE(rec.isLive(kPlay));
}

// 13. Seek forward past punch-out with a stale touch: release should be at
// lastChange+release with glide, not held to punch-out.
TEST(AutomationEdgeCaseTest, PunchOutPreemptsTouchRelease) {
    LaneMap lanes; lanes.emplace(kVol, ramp(kVol, 0, 0.5, 64, 0.5));
    AutomationRecorder rec(lanes);
    auto s = noThin(); s.punchEnabled = true; s.punchInBeat = 0; s.punchOutBeat = 32;
    rec.setSettings(s); rec.start(0);
    rec.controlChanged(kVol, 0.9, 2);
    rec.advance(40); // jump
    EXPECT_NEAR(0.5, lanes.at(kVol).valueAt(20, -1), 1e-6);
}
