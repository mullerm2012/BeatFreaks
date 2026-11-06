#include <gtest/gtest.h>

#include <vector>

#include "beatfreaks/automation/automationrecorder.h"

using namespace beatfreaks;

namespace {

const std::string kVolA = "[Channel1],volume";
const std::string kFilterC = "[Channel3],filter";
const std::string kPlayA = "[Channel1],play";

// Simulate a fader move: value goes from `from` to `to` between two beats,
// with a change every 1/16 beat, advancing the clock as a real session would.
void sweep(AutomationRecorder& rec,
        const std::string& key,
        double fromBeat,
        double toBeat,
        double fromValue,
        double toValue) {
    const double step = 1.0 / 16.0;
    for (double b = fromBeat; b <= toBeat + 1e-9; b += step) {
        const double t = (b - fromBeat) / (toBeat - fromBeat);
        rec.advance(b);
        rec.controlChanged(key, fromValue + (toValue - fromValue) * t, b);
    }
}

} // namespace

TEST(AutomationRecorderTest, NothingRecordedWhenNotStarted) {
    LaneMap lanes;
    AutomationRecorder rec(lanes);
    EXPECT_FALSE(rec.controlChanged(kVolA, 0.5, 1.0));
    EXPECT_TRUE(lanes.empty());
}

TEST(AutomationRecorderTest, ReadModeNeverRecords) {
    LaneMap lanes;
    AutomationRecorder rec(lanes);
    RecorderSettings s;
    s.mode = AutomationMode::Read;
    rec.setSettings(s);
    rec.start(0.0);
    EXPECT_FALSE(rec.isRecording());
    EXPECT_FALSE(rec.controlChanged(kVolA, 0.5, 1.0));
}

TEST(AutomationRecorderTest, RecordsAFaderMoveAndThinsIt) {
    LaneMap lanes;
    AutomationRecorder rec(lanes);
    rec.start(0.0);
    sweep(rec, kVolA, 32.0, 40.0, 0.8, 0.0); // fade out A over 2 bars
    rec.stop(48.0);
    ASSERT_EQ(1u, lanes.count(kVolA));
    const AutomationLane& lane = lanes.at(kVolA);
    EXPECT_LT(lane.size(), 10u); // a straight fade thins to a few points
    EXPECT_NEAR(0.8, lane.valueAt(32.0, -1), 1e-3);
    EXPECT_NEAR(0.4, lane.valueAt(36.0, -1), 5e-3);
    EXPECT_NEAR(0.0, lane.valueAt(40.0, -1), 1e-3);
    EXPECT_NEAR(0.0, lane.valueAt(47.0, -1), 1e-3);
}

TEST(AutomationRecorderTest, TouchReleasesBackToOldTake) {
    LaneMap lanes;
    // Old take: filter centred (0.5) for the whole set.
    lanes.emplace(kFilterC, AutomationLane(kFilterC));
    lanes.at(kFilterC).insert(0.0, 0.5);
    lanes.at(kFilterC).insert(64.0, 0.5);

    AutomationRecorder rec(lanes);
    RecorderSettings s;
    s.touchReleaseBeats = 8.0;
    rec.setSettings(s);
    rec.start(0.0);
    sweep(rec, kFilterC, 16.0, 20.0, 0.5, 0.9);
    EXPECT_TRUE(rec.isLive(kFilterC));
    // The DJ lets go at beat 20. Keep the clock running.
    for (double b = 20.0; b <= 40.0; b += 0.25) {
        rec.advance(b);
    }
    EXPECT_FALSE(rec.isLive(kFilterC)); // released after 8 beats of rest
    rec.stop(64.0);

    const AutomationLane& lane = lanes.at(kFilterC);
    EXPECT_NEAR(0.5, lane.valueAt(8.0, -1), 1e-3);  // before the touch: old take
    EXPECT_NEAR(0.7, lane.valueAt(18.0, -1), 5e-3); // during the sweep
    EXPECT_NEAR(0.9, lane.valueAt(26.0, -1), 1e-3); // held while resting
    EXPECT_NEAR(0.9, lane.valueAt(28.0, -1), 1e-3); // release point (20 + 8)
    EXPECT_NEAR(0.5, lane.valueAt(28.5, -1), 1e-3); // glided back in half a beat
    EXPECT_NEAR(0.5, lane.valueAt(40.0, -1), 1e-3); // old take resumes
    EXPECT_NEAR(0.5, lane.valueAt(64.0, -1), 1e-3);
}

TEST(AutomationRecorderTest, LatchHoldsValueUntilStop) {
    LaneMap lanes;
    lanes.emplace(kVolA, AutomationLane(kVolA));
    lanes.at(kVolA).insert(0.0, 0.2);
    lanes.at(kVolA).insert(30.0, 1.0); // old take had a move here

    AutomationRecorder rec(lanes);
    RecorderSettings s;
    s.mode = AutomationMode::Latch;
    rec.setSettings(s);
    rec.start(0.0);
    sweep(rec, kVolA, 8.0, 10.0, 0.2, 0.6);
    for (double b = 10.0; b <= 40.0; b += 0.5) {
        rec.advance(b);
    }
    EXPECT_TRUE(rec.isLive(kVolA)); // latch never releases on its own
    rec.stop(40.0);
    EXPECT_FALSE(rec.isLive(kVolA));
    const AutomationLane& lane = lanes.at(kVolA);
    EXPECT_NEAR(0.6, lane.valueAt(30.0, -1), 1e-3); // old move overwritten
    EXPECT_NEAR(0.6, lane.valueAt(40.0, -1), 1e-3);
}

TEST(AutomationRecorderTest, PunchRangeLimitsRecording) {
    LaneMap lanes;
    AutomationRecorder rec(lanes);
    RecorderSettings s;
    s.punchEnabled = true;
    s.punchInBeat = 128.0;
    s.punchOutBeat = 160.0;
    rec.setSettings(s);
    rec.start(100.0);
    EXPECT_FALSE(rec.controlChanged(kVolA, 0.3, 120.0)); // before punch-in
    EXPECT_TRUE(rec.controlChanged(kVolA, 0.4, 130.0));
    rec.advance(170.0); // past punch-out closes the take
    EXPECT_FALSE(rec.isLive(kVolA));
    EXPECT_FALSE(rec.controlChanged(kVolA, 0.9, 170.0));
    rec.stop(180.0);
    const AutomationLane& lane = lanes.at(kVolA);
    EXPECT_NEAR(0.4, lane.valueAt(130.0, -1), 1e-6);
    EXPECT_LE(lane.points().back().beat, 160.0);
}

TEST(AutomationRecorderTest, OnlyArmedChannelsRecord) {
    LaneMap lanes;
    AutomationRecorder rec(lanes);
    RecorderSettings s;
    s.armedGroups = {"[Channel3]"};
    rec.setSettings(s);
    rec.start(0.0);
    EXPECT_FALSE(rec.controlChanged(kVolA, 0.5, 1.0));
    EXPECT_TRUE(rec.controlChanged(kFilterC, 0.5, 1.0));
    EXPECT_EQ(0u, lanes.count(kVolA));
    EXPECT_EQ(1u, lanes.count(kFilterC));
}

TEST(AutomationRecorderTest, ButtonsAreRecordedAsSteps) {
    LaneMap lanes;
    AutomationRecorder rec(lanes);
    rec.start(0.0);
    rec.controlChanged(kPlayA, 1.0, 4.0, true);
    rec.controlChanged(kPlayA, 0.0, 12.0, true);
    rec.stop(16.0);
    const AutomationLane& lane = lanes.at(kPlayA);
    EXPECT_TRUE(lane.isToggle());
    EXPECT_FALSE(rec.isLive(kPlayA));
    EXPECT_DOUBLE_EQ(1.0, lane.valueAt(11.9, -1));
    EXPECT_DOUBLE_EQ(0.0, lane.valueAt(12.0, -1));
}

TEST(AutomationPlayerTest, SendsOnlyChangesAndSkipsLiveControls) {
    LaneMap lanes;
    lanes.emplace(kVolA, AutomationLane(kVolA));
    lanes.at(kVolA).insert(0.0, 0.0);
    lanes.at(kVolA).insert(8.0, 1.0);
    lanes.emplace(kFilterC, AutomationLane(kFilterC));
    lanes.at(kFilterC).insert(0.0, 0.5);

    AutomationRecorder rec(lanes);
    AutomationPlayer player(lanes, &rec);
    std::vector<std::pair<std::string, double>> sent;
    auto write = [&](const std::string& k, double v) { sent.emplace_back(k, v); };

    EXPECT_EQ(2u, player.process(0.0, write)); // first call sends everything
    sent.clear();
    EXPECT_EQ(1u, player.process(4.0, write)); // filter unchanged, volume moved
    ASSERT_EQ(1u, sent.size());
    EXPECT_EQ(kVolA, sent[0].first);
    EXPECT_NEAR(0.5, sent[0].second, 1e-9);

    // The DJ grabs the volume fader: playback must not fight it.
    rec.start(4.0);
    rec.controlChanged(kVolA, 0.2, 4.5);
    sent.clear();
    player.process(5.0, write);
    EXPECT_TRUE(sent.empty());

    // After a seek everything is re-sent.
    rec.stop(6.0);
    player.reset();
    sent.clear();
    EXPECT_EQ(2u, player.process(1.0, write));
}
