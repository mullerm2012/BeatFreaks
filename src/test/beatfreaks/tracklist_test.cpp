#include <gtest/gtest.h>

#include <QJsonArray>
#include <limits>

#include "beatfreaks/library/tracklist.h"

using namespace beatfreaks;

namespace {

PlayedTrack song(const char* artist, const char* title, double start, double played = 0,
        double bpm = 0, const char* key = "", const char* art = "") {
    PlayedTrack t;
    t.artist = QString::fromUtf8(artist);
    t.title = QString::fromUtf8(title);
    t.filePath = QStringLiteral("/music/%1 - %2.mp3").arg(t.artist, t.title);
    t.startSeconds = start;
    t.playedSeconds = played;
    t.bpm = bpm;
    t.key = QString::fromUtf8(key);
    t.coverArtPath = QString::fromUtf8(art);
    return t;
}

QList<PlayedTrack> set() {
    return {
            song("Alpha", "One", 0, 300, 126, "8A", "/art/a.jpg"),
            song("Beta", "Two", 290, 330, 127, "9A", "/art/b.jpg"),
            song("Gamma", "Three", 610, 400, 128, "9A", "/art/c.jpg"),
            song("Delta", "Four", 1000, 0, 128, "10A", "/art/d.jpg"),
    };
}

} // namespace

TEST(Tracklist, FormatTime) {
    EXPECT_EQ(Tracklist::formatTime(0), QStringLiteral("00:00"));
    EXPECT_EQ(Tracklist::formatTime(65.9), QStringLiteral("01:05"));
    EXPECT_EQ(Tracklist::formatTime(3723), QStringLiteral("1:02:03"));
    EXPECT_EQ(Tracklist::formatTime(5, true), QStringLiteral("0:00:05"));
    EXPECT_EQ(Tracklist::formatTime(-4), QStringLiteral("00:00"));
}

TEST(Tracklist, SortsDropsPreviewsAndRepeats) {
    QList<PlayedTrack> played = set();
    played.insert(1, song("Oops", "Preview", 280, 8));        // 8 s on air: dropped
    played.append(song("Delta", "Four", 1005, 0));             // restarted: merged
    std::reverse(played.begin(), played.end());                // order from DB may vary
    const Tracklist list(played);
    ASSERT_EQ(list.tracks().size(), 4);
    EXPECT_EQ(list.tracks()[0].title, QStringLiteral("One"));
    EXPECT_EQ(list.tracks()[3].title, QStringLiteral("Four"));
}

TEST(Tracklist, UnknownLengthUsesGapToNextSong) {
    // 20 s gap → too short; the last song is always kept
    const Tracklist list({song("A", "x", 0, 0), song("B", "y", 20, 0), song("C", "z", 200, 0)});
    ASSERT_EQ(list.tracks().size(), 2);
    EXPECT_EQ(list.tracks()[0].artist, QStringLiteral("B"));
    EXPECT_EQ(list.tracks()[1].artist, QStringLiteral("C"));
}

TEST(Tracklist, TextHasNumbersTimesKeysAndCredit) {
    TracklistOptions o;
    o.mixTitle = QStringLiteral("Freak Friday");
    o.djName = QStringLiteral("mullerm2012");
    o.date = QStringLiteral("2026-11-06");
    QList<PlayedTrack> played = set();
    played[2].aiMade = true;
    const QString text = Tracklist(played, o).toText();
    EXPECT_TRUE(text.startsWith(QStringLiteral("Freak Friday\nmullerm2012 · 2026-11-06\n\n")));
    EXPECT_TRUE(text.contains(QStringLiteral("01. [00:00] Alpha – One (8A, 126 BPM)")));
    EXPECT_TRUE(text.contains(QStringLiteral("03. [10:10] Gamma – Three (9A, 128 BPM, AI-made)")));
    EXPECT_TRUE(text.endsWith(QStringLiteral("Mixed live in BeatFreaks")));
}

TEST(Tracklist, HoursWhenTheSetIsLong) {
    const Tracklist list({song("A", "x", 0, 4000), song("B", "y", 3700, 300)});
    EXPECT_TRUE(list.toText().contains(QStringLiteral("[0:00:00] A – x")));
    EXPECT_TRUE(list.toText().contains(QStringLiteral("[1:01:40] B – y")));
}

TEST(Tracklist, YouTubeChapters) {
    const QString ch = Tracklist(set()).toYouTubeChapters();
    const QStringList lines = ch.split(QLatin1Char('\n'));
    ASSERT_EQ(lines.size(), 4);
    EXPECT_EQ(lines[0], QStringLiteral("00:00 Alpha – One"));
    EXPECT_EQ(lines[1], QStringLiteral("04:50 Beta – Two"));
    // fewer than 3 chapters: YouTube would ignore them
    EXPECT_TRUE(Tracklist({song("A", "x", 0, 100), song("B", "y", 100, 100)}).toYouTubeChapters().isEmpty());
}

TEST(Tracklist, YouTubeChaptersAreTenSecondsApart) {
    TracklistOptions o;
    o.minPlaySeconds = 0;
    const Tracklist list({song("A", "x", 5, 100), song("B", "y", 8, 100), song("C", "z", 200, 100),
                                 song("D", "w", 400, 100)},
            o);
    const QStringList lines = list.toYouTubeChapters().split(QLatin1Char('\n'));
    ASSERT_EQ(lines.size(), 3);
    EXPECT_EQ(lines[0], QStringLiteral("00:00 A – x")); // first moved to 0:00
    EXPECT_EQ(lines[1], QStringLiteral("03:20 C – z")); // B was only 8 s after 0:00
}

TEST(Tracklist, CueSheet) {
    TracklistOptions o;
    o.djName = QStringLiteral("DJ \"Freak\"");
    const QString cue = Tracklist(set(), o).toCue(QStringLiteral("C:/Mixes/friday.mp3"));
    EXPECT_TRUE(cue.contains(QStringLiteral("PERFORMER \"DJ 'Freak'\"")));
    EXPECT_TRUE(cue.contains(QStringLiteral("FILE \"friday.mp3\" MP3")));
    EXPECT_TRUE(cue.contains(QStringLiteral("  TRACK 01 AUDIO\n    TITLE \"One\"\n    PERFORMER \"Alpha\"\n    INDEX 01 00:00:00")));
    EXPECT_TRUE(cue.contains(QStringLiteral("  TRACK 02 AUDIO")));
    EXPECT_TRUE(cue.contains(QStringLiteral("INDEX 01 04:50:00")));
    EXPECT_TRUE(cue.contains(QStringLiteral("INDEX 01 16:40:00")));

    // fractions of a second become CUE frames (1/75 s); minutes may pass 99
    TracklistOptions keepAll;
    keepAll.minPlaySeconds = 0;
    const QString long_ = Tracklist({song("A", "x", 0, 1), song("B", "y", 6001.5, 1)}, keepAll)
                                  .toCue(QStringLiteral("mix.wav"));
    EXPECT_TRUE(long_.contains(QStringLiteral("FILE \"mix.wav\" WAVE")));
    EXPECT_TRUE(long_.contains(QStringLiteral("INDEX 01 100:01:38")));
}

TEST(Tracklist, CsvEscapes) {
    const QString csv = Tracklist({song("Smith, J", "Say \"hi\"", 0, 100, 124.5, "8A")}).toCsv();
    EXPECT_EQ(csv, QStringLiteral("#,Start,Artist,Title,Key,BPM,AI-made\n"
                                  "1,0:00:00,\"Smith, J\",\"Say \"\"hi\"\"\",8A,124.5,no\n"));
}

TEST(Tracklist, CsvBlocksSpreadsheetFormulas) {
    TracklistOptions keepAll;
    keepAll.minPlaySeconds = 0;
    const QString csv = Tracklist({song("=HYPERLINK(\"x\")", "+1", 0, 100, 0, "-8A"),
                                          song("@SUM(A1)", "\t=cmd", 100, 100)},
            keepAll)
                                .toCsv();
    EXPECT_TRUE(csv.contains(QStringLiteral("\"'=HYPERLINK(\"\"x\"\")\"")));
    EXPECT_TRUE(csv.contains(QStringLiteral(",'+1,'-8A,")));
    EXPECT_TRUE(csv.contains(QStringLiteral("'@SUM(A1)")));
    EXPECT_TRUE(csv.contains(QStringLiteral("'\t=cmd")));
    EXPECT_FALSE(csv.contains(QStringLiteral(",=")));
}

TEST(Tracklist, BadTimestampsAndUnknownLastLength) {
    PlayedTrack a = song("A", "x", std::numeric_limits<double>::quiet_NaN(), 120);
    PlayedTrack b = song("B", "y", -50, 0);
    PlayedTrack c = song("C", "z", 300, 0); // still playing: length unknown
    const Tracklist list({a, b, c});
    // bad times become 0:00; input order is kept for equal times
    ASSERT_EQ(list.tracks().size(), 3);
    EXPECT_EQ(list.tracks()[0].artist, QStringLiteral("A"));
    EXPECT_DOUBLE_EQ(list.tracks()[0].startSeconds, 0.0);
    EXPECT_DOUBLE_EQ(list.tracks()[1].startSeconds, 0.0);
    EXPECT_EQ(list.tracks().last().artist, QStringLiteral("C"));
    EXPECT_DOUBLE_EQ(list.stats().lengthSeconds, 300.0);
    EXPECT_FALSE(list.toText().contains(QStringLiteral("nan")));
    EXPECT_TRUE(list.toCue(QStringLiteral("m.wav")).contains(QStringLiteral("INDEX 01 05:00:00")));
}

TEST(Tracklist, StatsAndJson) {
    QList<PlayedTrack> played = set();
    played[1].aiMade = true;
    const Tracklist list(played);
    const TracklistStats st = list.stats();
    EXPECT_EQ(st.tracks, 4);
    EXPECT_DOUBLE_EQ(st.minBpm, 126);
    EXPECT_DOUBLE_EQ(st.maxBpm, 128);
    EXPECT_EQ(st.keyJourney, (QStringList{"8A", "9A", "10A"}));
    EXPECT_EQ(st.aiMade, 1);
    EXPECT_DOUBLE_EQ(st.lengthSeconds, 1000);
    EXPECT_DOUBLE_EQ(list.stats(3600).lengthSeconds, 3600);
    const QJsonObject j = list.toJson();
    EXPECT_EQ(j.value(QStringLiteral("tracks")).toArray().size(), 4);
    EXPECT_EQ(j.value(QStringLiteral("key_journey")).toArray().size(), 3);
}

TEST(Tracklist, NamesWithoutTags) {
    PlayedTrack t;
    t.filePath = QStringLiteral("/music/untagged banger.mp3");
    EXPECT_EQ(Tracklist::displayName(t), QStringLiteral("untagged banger"));
    EXPECT_EQ(Tracklist::displayName(PlayedTrack{}), QStringLiteral("Unknown"));
}

TEST(MixCover, AlbumArtIsOptIn) {
    // default: no album art (publishing rights), BeatFreaks art + title band
    const MixCoverPlan p = MixCoverPlan::make(Tracklist(set()), {}, 3000);
    EXPECT_EQ(p.grid, 0);
    EXPECT_TRUE(p.tiles.isEmpty());
    EXPECT_EQ(p.titleBandY + p.titleBandHeight, 3000);
}

TEST(MixCover, GridFromArtwork) {
    TracklistOptions o;
    o.useTrackArtwork = true;
    o.mixTitle = QStringLiteral("Freak Friday");
    o.djName = QStringLiteral("mullerm2012");
    const MixCoverPlan four = MixCoverPlan::make(Tracklist(set()), o, 3000);
    EXPECT_EQ(four.grid, 2);
    ASSERT_EQ(four.tiles.size(), 4);
    // longest on air first: Gamma (400 s)
    EXPECT_EQ(four.tiles[0].artPath, QStringLiteral("/art/c.jpg"));
    EXPECT_EQ(four.tiles[3].x, 1500);
    EXPECT_EQ(four.tiles[3].y, 1500);
    EXPECT_EQ(four.tiles[3].size, 1500);
    EXPECT_EQ(four.title, QStringLiteral("Freak Friday"));
    EXPECT_EQ(four.subtitle, QStringLiteral("mullerm2012 · 4 tracks"));
    EXPECT_EQ(four.titleBandY + four.titleBandHeight, 3000);

    QList<PlayedTrack> three = set().mid(0, 3);
    EXPECT_EQ(MixCoverPlan::make(Tracklist(three), o).grid, 1);

    QList<PlayedTrack> none = set();
    for (PlayedTrack& t : none) {
        t.coverArtPath.clear();
    }
    const MixCoverPlan bare = MixCoverPlan::make(Tracklist(none), {}, 3000);
    EXPECT_EQ(bare.grid, 0);
    EXPECT_TRUE(bare.tiles.isEmpty());
    EXPECT_EQ(bare.title, QStringLiteral("BeatFreaks Mix"));
}

TEST(MixCover, NineTilesMax) {
    QList<PlayedTrack> many;
    for (int i = 0; i < 12; ++i) {
        many << song("A", QByteArray::number(i).constData(), i * 100.0, 100, 0, "",
                QByteArray("/art/" + QByteArray::number(i) + ".jpg").constData());
    }
    TracklistOptions o;
    o.useTrackArtwork = true;
    const MixCoverPlan p = MixCoverPlan::make(Tracklist(many), o, 900);
    EXPECT_EQ(p.grid, 3);
    EXPECT_EQ(p.tiles.size(), 9);
    EXPECT_EQ(p.tiles[8].x, 600);
}
