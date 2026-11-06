#include <gtest/gtest.h>

#include "beatfreaks/automation/automationlane.h"

using namespace beatfreaks;

TEST(AutomationLaneTest, EmptyLaneReturnsFallback) {
    AutomationLane lane("[Channel1],volume");
    EXPECT_TRUE(lane.isEmpty());
    EXPECT_DOUBLE_EQ(0.7, lane.valueAt(12.0, 0.7));
    EXPECT_EQ("[Channel1]", lane.group());
}

TEST(AutomationLaneTest, InsertKeepsOrderAndReplacesSameBeat) {
    AutomationLane lane("[Channel1],volume");
    lane.insert(8.0, 0.5);
    lane.insert(0.0, 1.0);
    lane.insert(4.0, 0.2);
    lane.insert(4.0, 0.3); // replaces
    ASSERT_EQ(3u, lane.size());
    EXPECT_DOUBLE_EQ(0.0, lane.points()[0].beat);
    EXPECT_DOUBLE_EQ(4.0, lane.points()[1].beat);
    EXPECT_DOUBLE_EQ(0.3, lane.points()[1].value);
    EXPECT_DOUBLE_EQ(8.0, lane.points()[2].beat);
}

TEST(AutomationLaneTest, LinearInterpolationAndEdges) {
    AutomationLane lane("[Channel1],volume");
    lane.insert(4.0, 0.0);
    lane.insert(8.0, 1.0);
    EXPECT_DOUBLE_EQ(0.0, lane.valueAt(0.0, 9.0));  // before first: first value
    EXPECT_DOUBLE_EQ(0.5, lane.valueAt(6.0, 9.0));  // halfway
    EXPECT_DOUBLE_EQ(0.25, lane.valueAt(5.0, 9.0));
    EXPECT_DOUBLE_EQ(1.0, lane.valueAt(100.0, 9.0)); // after last: last value
}

TEST(AutomationLaneTest, ToggleLanesHoldUntilNextPoint) {
    AutomationLane lane("[Channel1],play", true);
    lane.insert(0.0, 1.0);
    lane.insert(16.0, 0.0);
    EXPECT_EQ(CurveShape::Hold, lane.points()[0].shape);
    EXPECT_DOUBLE_EQ(1.0, lane.valueAt(15.99, 0.0));
    EXPECT_DOUBLE_EQ(0.0, lane.valueAt(16.0, 1.0));
}

TEST(AutomationLaneTest, EraseRangeIsOpenOnTheLeft) {
    AutomationLane lane("[Channel1],volume");
    for (int b = 0; b <= 10; ++b) {
        lane.insert(b, b / 10.0);
    }
    EXPECT_EQ(3u, lane.eraseRange(2.0, 5.0)); // removes 3, 4, 5
    ASSERT_EQ(8u, lane.size());
    EXPECT_DOUBLE_EQ(2.0, lane.points()[2].beat);
    EXPECT_DOUBLE_EQ(6.0, lane.points()[3].beat);
    EXPECT_EQ(2u, lane.eraseRangeInclusive(6.0, 7.0));
    EXPECT_EQ(0u, lane.eraseRange(5.0, 1.0)); // backwards range does nothing
}

TEST(AutomationLaneTest, ShiftMovesTailAndDropsNegative) {
    AutomationLane lane("[Channel1],volume");
    lane.insert(0.0, 0.1);
    lane.insert(4.0, 0.4);
    lane.insert(8.0, 0.8);
    lane.shift(4.0, 2.0);
    ASSERT_EQ(3u, lane.size());
    EXPECT_DOUBLE_EQ(6.0, lane.points()[1].beat);
    EXPECT_DOUBLE_EQ(10.0, lane.points()[2].beat);
    lane.shift(0.0, -7.0); // 0 -> -7 dropped, 6 -> -1 dropped, 10 -> 3
    ASSERT_EQ(1u, lane.size());
    EXPECT_DOUBLE_EQ(3.0, lane.points()[0].beat);
}

TEST(AutomationLaneTest, ShiftRangeMovesOnlyTheBlockAndReplacesLanding) {
    AutomationLane lane("[Channel1],filter");
    lane.insert(0.0, 0.5);
    lane.insert(10.0, 0.9); // in the moving block
    lane.insert(12.0, 0.1); // in the moving block
    lane.insert(20.0, 0.3); // in the landing zone, should be replaced
    lane.insert(40.0, 0.5);
    lane.shiftRange(8.0, 16.0, 10.0); // block 8..16 lands on 18..26
    ASSERT_EQ(4u, lane.size());
    EXPECT_DOUBLE_EQ(0.0, lane.points()[0].beat);
    EXPECT_DOUBLE_EQ(20.0, lane.points()[1].beat);
    EXPECT_DOUBLE_EQ(0.9, lane.points()[1].value);
    EXPECT_DOUBLE_EQ(22.0, lane.points()[2].beat);
    EXPECT_DOUBLE_EQ(40.0, lane.points()[3].beat);
}

TEST(AutomationLaneTest, ThinRemovesCollinearPointsKeepsCorners) {
    AutomationLane lane("[Channel1],volume");
    // A ramp up 0..1 over beats 0..10, flat until 20, sampled every 0.1 beat
    // like a live fader recording.
    for (int i = 0; i <= 200; ++i) {
        const double beat = i * 0.1;
        lane.insert(beat, beat < 10.0 ? beat / 10.0 : 1.0);
    }
    const std::size_t before = lane.size();
    lane.thin(0.001);
    EXPECT_LT(lane.size(), 6u);
    EXPECT_GT(before, 150u);
    // The curve is unchanged within tolerance.
    for (double b = 0.0; b <= 20.0; b += 0.37) {
        const double expected = b < 10.0 ? b / 10.0 : 1.0;
        EXPECT_NEAR(expected, lane.valueAt(b, -1.0), 0.002) << "at beat " << b;
    }
}

TEST(AutomationLaneTest, ThinNeverDropsHoldSteps) {
    AutomationLane lane("[Channel1],volume");
    lane.insert(AutomationPoint{0.0, 0.0, CurveShape::Linear});
    lane.insert(AutomationPoint{1.0, 0.0, CurveShape::Hold});
    lane.insert(AutomationPoint{2.0, 1.0, CurveShape::Linear});
    lane.insert(AutomationPoint{3.0, 1.0, CurveShape::Linear});
    lane.thin(0.5);
    EXPECT_EQ(4u, lane.size());
}
