#include <gtest/gtest.h>

#include <QJsonObject>

#include "beatfreaks/ai/songbrief.h"

using namespace beatfreaks;
using namespace beatfreaks::ai;

namespace {

FloorState floorAt(double bpm, const char* key, double energy, double heat = -1.0) {
    FloorState f;
    f.bpm = bpm;
    f.key = CamelotKey::parse(key);
    f.energy = energy;
    f.floorHeat = heat;
    f.genresThatWorked = {QStringLiteral("tech house"), QStringLiteral("house")};
    return f;
}

} // namespace

TEST(SongBrief, CamelotToKeyScale) {
    EXPECT_EQ(camelotToKeyScale(CamelotKey::parse("8A")), QStringLiteral("A minor"));
    EXPECT_EQ(camelotToKeyScale(CamelotKey::parse("8B")), QStringLiteral("C major"));
    EXPECT_EQ(camelotToKeyScale(CamelotKey::parse("1A")), QStringLiteral("G# minor"));
    EXPECT_EQ(camelotToKeyScale(CamelotKey::parse("12B")), QStringLiteral("E major"));
    EXPECT_EQ(camelotToKeyScale(CamelotKey::parse("5A")), QStringLiteral("C minor"));
    EXPECT_TRUE(camelotToKeyScale(CamelotKey{}).isEmpty());
}

TEST(SongBrief, BarsToSeconds) {
    EXPECT_DOUBLE_EQ(barsToSeconds(128, 128.0), 240.0);
    EXPECT_DOUBLE_EQ(barsToSeconds(32, 120.0), 64.0);
    EXPECT_DOUBLE_EQ(barsToSeconds(32, 0.0), 0.0);
    EXPECT_DOUBLE_EQ(barsToSeconds(0, 120.0), 0.0);
}

TEST(SongBrief, FillTheFloorStaysMixable) {
    const SongBrief b = SongBrief::fromFloor(floorAt(127.6, "8A", 6.0), SongPurpose::FillTheFloor);
    EXPECT_EQ(b.genre, QStringLiteral("tech house"));
    EXPECT_DOUBLE_EQ(b.bpm, 128.0);
    EXPECT_EQ(b.key.toString(), "8A");
    EXPECT_DOUBLE_EQ(b.energy, 7.0);
    EXPECT_EQ(b.vocals, Vocals::Hook);
}

TEST(SongBrief, ColdFloorGetsABiggerPush) {
    const SongBrief warm = SongBrief::fromFloor(floorAt(124, "8A", 6.0, 0.8), SongPurpose::FillTheFloor);
    const SongBrief cold = SongBrief::fromFloor(floorAt(124, "8A", 6.0, 0.1), SongPurpose::FillTheFloor);
    EXPECT_GT(cold.energy, warm.energy);
}

TEST(SongBrief, PeakBangerUsesEnergyBoostKeyAndWraps) {
    const SongBrief b = SongBrief::fromFloor(floorAt(128, "12A", 6.0), SongPurpose::PeakBanger);
    EXPECT_EQ(b.key.toString(), "1A");
    EXPECT_GE(b.energy, 8.5);
    EXPECT_LE(b.energy, 10.0);
}

TEST(SongBrief, CoolDownUsesRelativeKey) {
    const SongBrief b = SongBrief::fromFloor(floorAt(124, "8A", 7.0), SongPurpose::CoolDown);
    EXPECT_EQ(b.key.toString(), "8B");
    EXPECT_DOUBLE_EQ(b.energy, 5.0);
}

TEST(SongBrief, ToolTrackIsInstrumentalAndShorter) {
    const SongBrief b = SongBrief::fromFloor(floorAt(126, "4A", 6.0), SongPurpose::ToolTrack);
    EXPECT_EQ(b.vocals, Vocals::None);
    EXPECT_EQ(b.totalBars, 96);
    EXPECT_FALSE(b.toAceStepRequest().value(QStringLiteral("use_format")).toBool());
    EXPECT_TRUE(b.structuredLyrics().contains(QStringLiteral("[Instrumental]")));
}

TEST(SongBrief, UnknownFloorFallsBackToSaneDefaults) {
    const SongBrief b = SongBrief::fromFloor(FloorState{}, SongPurpose::FillTheFloor);
    EXPECT_EQ(b.genre, QStringLiteral("house"));
    EXPECT_DOUBLE_EQ(b.bpm, 124.0);
    EXPECT_FALSE(b.key.isValid());
    EXPECT_FALSE(b.toAceStepRequest().contains(QStringLiteral("key_scale")));
}

TEST(SongBrief, AceStepRequestHasEverythingTheServerNeeds) {
    SongBrief b = SongBrief::fromFloor(floorAt(128, "8A", 6.0), SongPurpose::FillTheFloor);
    b.variations = 20;
    const QJsonObject r = b.toAceStepRequest();
    EXPECT_EQ(r.value(QStringLiteral("bpm")).toInt(), 128);
    EXPECT_EQ(r.value(QStringLiteral("key_scale")).toString(), QStringLiteral("A minor"));
    EXPECT_EQ(r.value(QStringLiteral("time_signature")).toString(), QStringLiteral("4"));
    EXPECT_DOUBLE_EQ(r.value(QStringLiteral("audio_duration")).toDouble(), 240.0);
    EXPECT_EQ(r.value(QStringLiteral("batch_size")).toInt(), 8);
    EXPECT_TRUE(r.value(QStringLiteral("thinking")).toBool());
    EXPECT_TRUE(r.value(QStringLiteral("use_format")).toBool()); // model writes the words
    const QString lyrics = r.value(QStringLiteral("lyrics")).toString();
    EXPECT_TRUE(lyrics.startsWith(QStringLiteral("[Intro - 32 bars, drums only]")));
    EXPECT_TRUE(lyrics.endsWith(QStringLiteral("[Outro - 32 bars, drums only]")));
    // the theme is a style hint, never a line to sing
    EXPECT_FALSE(lyrics.contains(b.theme));
    EXPECT_TRUE(r.value(QStringLiteral("prompt")).toString().contains(b.theme));
    b.variations = 0;
    EXPECT_EQ(b.toAceStepRequest().value(QStringLiteral("batch_size")).toInt(), 1);
}

TEST(SongBrief, OwnLyricsAreKeptAndNotRewritten) {
    SongBrief b;
    b.lyrics = QStringLiteral("Freaks on the floor\nBeat goes on");
    const QJsonObject r = b.toAceStepRequest();
    EXPECT_FALSE(r.value(QStringLiteral("use_format")).toBool());
    const QString lyrics = r.value(QStringLiteral("lyrics")).toString();
    EXPECT_TRUE(lyrics.contains(QStringLiteral("[Verse]\nFreaks on the floor")));
    EXPECT_TRUE(lyrics.contains(QStringLiteral("[Drop]")));

    b.lyrics = QStringLiteral("[Hook]\nHey\n[Drop]");
    const QString tagged = b.structuredLyrics();
    EXPECT_FALSE(tagged.contains(QStringLiteral("[Verse]")));
    EXPECT_EQ(tagged.count(QStringLiteral("[Drop]")), 1);
}

TEST(SongBrief, IntroAndOutroSnapToPhrases) {
    SongBrief b;
    b.introBars = 30;
    b.outroBars = 3;
    const QString l = b.structuredLyrics();
    EXPECT_TRUE(l.contains(QStringLiteral("[Intro - 32 bars")));
    EXPECT_TRUE(l.contains(QStringLiteral("[Outro - 8 bars")));
}

TEST(SongBrief, DurationIsClamped) {
    SongBrief b;
    b.bpm = 60;
    b.totalBars = 1000;
    EXPECT_DOUBLE_EQ(b.durationSeconds(), 600.0);
    b.bpm = 300;
    b.totalBars = 1;
    EXPECT_DOUBLE_EQ(b.durationSeconds(), 10.0);
}

TEST(SongBrief, SunoPromptFitsTheStyleBox) {
    SongBrief b = SongBrief::fromFloor(floorAt(128, "8A", 6.0), SongPurpose::PeakBanger);
    b.extraStyle = QString(300, QLatin1Char('x'));
    const QString p = b.toSunoPrompt();
    const QString style = p.section(QStringLiteral("\n\nLyrics:\n"), 0, 0).mid(7);
    EXPECT_LE(style.size(), 200);
    EXPECT_TRUE(p.contains(QStringLiteral("Lyrics:\n[Intro")));
}

TEST(SongBrief, Label) {
    const SongBrief b = SongBrief::fromFloor(floorAt(128, "8A", 6.0), SongPurpose::FillTheFloor);
    EXPECT_EQ(b.label(), QStringLiteral("tech house 128 8A"));
    EXPECT_EQ(purposeName(SongPurpose::PeakBanger), QStringLiteral("Peak banger"));
}
