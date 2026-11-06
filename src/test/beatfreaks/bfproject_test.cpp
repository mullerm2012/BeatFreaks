#include <gtest/gtest.h>

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QTemporaryDir>

#include "beatfreaks/diagnostics/diagnosticreport.h"
#include "beatfreaks/diagnostics/hardwaretest.h"
#include "beatfreaks/project/bfproject.h"

using namespace beatfreaks;

namespace {

BfProject sampleProject() {
    BfProject p;
    p.title = QStringLiteral("Friday Set – take 3");
    p.bpm = 124.0;
    p.clips.append(Clip{QStringLiteral("clip-1"),
            0,
            QStringLiteral("C:/Music/Glass Horizon.flac"),
            QStringLiteral("Glass Horizon"),
            0.0,
            168.0,
            0.0,
            0.0,
            0.0,
            false});
    p.clips.append(Clip{QStringLiteral("clip-2"),
            2,
            QStringLiteral("C:/Music/Copperline.wav"),
            QStringLiteral("Copperline (vocal stem)"),
            80.0,
            80.0,
            16.0,
            -1.5,
            0.0,
            false});
    p.markers.append(Marker{128.0, QStringLiteral("Bass swap"), QStringLiteral("#4DA3FF")});
    AutomationLane vol("[Channel1],volume");
    vol.insert(0.0, 0.8);
    vol.insert(136.0, 0.8);
    vol.insert(168.0, 0.0);
    p.lanes.emplace(vol.controlKey(), vol);
    AutomationLane filter("[Channel3],filter");
    filter.insert(80.0, 0.5);
    filter.insert(96.0, 0.85);
    filter.insert(120.0, 0.5);
    p.lanes.emplace(filter.controlKey(), filter);
    AutomationLane play("[Channel3],play", true);
    play.insert(80.0, 1.0);
    play.insert(160.0, 0.0);
    p.lanes.emplace(play.controlKey(), play);
    return p;
}

} // namespace

TEST(BfProjectTest, JsonRoundTripKeepsEverything) {
    const BfProject original = sampleProject();
    QString error;
    const auto loaded = BfProject::fromJson(original.toJson(), &error);
    ASSERT_TRUE(loaded.has_value()) << error.toStdString();
    EXPECT_EQ(original.title, loaded->title);
    EXPECT_DOUBLE_EQ(original.bpm, loaded->bpm);
    EXPECT_EQ(original.clips, loaded->clips);
    EXPECT_EQ(original.markers, loaded->markers);
    ASSERT_EQ(original.lanes.size(), loaded->lanes.size());
    for (const auto& [key, lane] : original.lanes) {
        ASSERT_EQ(1u, loaded->lanes.count(key)) << key;
        EXPECT_TRUE(lane == loaded->lanes.at(key)) << key;
    }
    EXPECT_EQ(CurveShape::Hold, loaded->lanes.at("[Channel3],play").points()[0].shape);
}

TEST(BfProjectTest, SaveAndLoadFromDisk) {
    QTemporaryDir dir;
    ASSERT_TRUE(dir.isValid());
    const QString path = dir.filePath(QStringLiteral("set.bfproj"));
    QString error;
    ASSERT_TRUE(sampleProject().save(path, &error)) << error.toStdString();
    const auto loaded = BfProject::load(path, &error);
    ASSERT_TRUE(loaded.has_value()) << error.toStdString();
    EXPECT_EQ(2, loaded->clips.size());
}

TEST(BfProjectTest, RejectsDamagedOrForeignFiles) {
    QTemporaryDir dir;
    const QString damaged = dir.filePath(QStringLiteral("bad.bfproj"));
    {
        QFile f(damaged);
        ASSERT_TRUE(f.open(QIODevice::WriteOnly));
        f.write("{ \"format\": \"beatfreaks-project\", ");
    }
    QString error;
    EXPECT_FALSE(BfProject::load(damaged, &error).has_value());
    EXPECT_TRUE(error.contains(QStringLiteral("damaged")));

    EXPECT_FALSE(BfProject::fromJson(QJsonObject{{"format", "something-else"}}, &error));
    EXPECT_FALSE(BfProject::fromJson(
            QJsonObject{{"format", "beatfreaks-project"}, {"version", 99}}, &error));
    EXPECT_TRUE(error.contains(QStringLiteral("99")));
}

TEST(BfProjectTest, MovingAClipCarriesItsAutomation) {
    BfProject p = sampleProject();
    ASSERT_TRUE(p.moveClip(QStringLiteral("clip-2"), 16.0)); // channel 3 clip, 80..160
    EXPECT_DOUBLE_EQ(96.0, p.clips[1].startBeat);
    const AutomationLane& filter = p.lanes.at("[Channel3],filter");
    EXPECT_DOUBLE_EQ(96.0, filter.points()[0].beat);
    EXPECT_DOUBLE_EQ(112.0, filter.points()[1].beat);
    // Channel 1 automation is untouched.
    EXPECT_DOUBLE_EQ(168.0, p.lanes.at("[Channel1],volume").points().back().beat);
    EXPECT_FALSE(p.moveClip(QStringLiteral("nope"), 4.0));
}

TEST(BfProjectTest, LengthCoversClipsAndLanes) {
    BfProject p = sampleProject();
    EXPECT_DOUBLE_EQ(168.0, p.lengthBeats());
    p.lanes.at("[Channel1],volume").insert(200.0, 0.0);
    EXPECT_DOUBLE_EQ(200.0, p.lengthBeats());
}

TEST(HardwareTestModelTest, CountsAndReport) {
    HardwareTestModel m = HardwareTestModel::forS4Mk3AndF1();
    EXPECT_GT(m.total("S4 MK3"), 80u);
    EXPECT_EQ(m.total("F1"), 16u + 12u + 10u);
    EXPECT_TRUE(m.markHeard("S4.L.play"));
    EXPECT_TRUE(m.markHeard("F1.pad7"));
    EXPECT_FALSE(m.markHeard("S4.L.does-not-exist"));
    EXPECT_TRUE(m.markFailed("F1.pad7", "LED did not light"));
    EXPECT_TRUE(m.markHeard("F1.pad7")); // stays failed
    EXPECT_EQ(1u, m.count(CheckState::Passed));
    EXPECT_EQ(1u, m.count(CheckState::Failed));
    const std::string report = m.report();
    EXPECT_NE(std::string::npos, report.find("1 of"));
    EXPECT_NE(std::string::npos, report.find("LED did not light"));
    EXPECT_NE(std::string::npos, report.find("NEVER HEARD"));
    m.reset();
    EXPECT_EQ(0u, m.count(CheckState::Passed));
    EXPECT_FALSE(m.allPassed());
}

TEST(DiagnosticReportTest, BundlesLogsAndHidesHomeFolder) {
    QTemporaryDir dir;
    ASSERT_TRUE(dir.isValid());
    {
        QFile log(QDir(dir.path()).filePath(QStringLiteral("mixxx.log")));
        ASSERT_TRUE(log.open(QIODevice::WriteOnly | QIODevice::Text));
        for (int i = 0; i < 50; ++i) {
            log.write(QStringLiteral("line %1 loaded /home/dj/Music/track.mp3\n")
                              .arg(i)
                              .toUtf8());
        }
    }
    DiagnosticReport::Input in;
    in.appVersion = QStringLiteral("0.1.0-test");
    in.settingsDir = dir.path();
    in.controllers = {QStringLiteral("Traktor Kontrol S4 MK3 (HID) - BeatFreaks mapping")};
    in.hardwareTestReport = QStringLiteral("Hardware Test: 90 of 93 controls passed");
    in.logTailLines = 10;
    in.homePath = QStringLiteral("/home/dj");
    const QDateTime now = QDateTime::fromString(QStringLiteral("2026-11-06T22:41:07"), Qt::ISODate);

    const QString text = DiagnosticReport::buildText(in, now);
    EXPECT_TRUE(text.contains(QStringLiteral("line 49")));
    EXPECT_FALSE(text.contains(QStringLiteral("line 39"))); // only the last 10 lines
    EXPECT_FALSE(text.contains(QStringLiteral("/home/dj")));
    EXPECT_TRUE(text.contains(QStringLiteral("~/Music/track.mp3")));
    EXPECT_TRUE(text.contains(QStringLiteral("90 of 93")));
    EXPECT_TRUE(text.contains(QStringLiteral("(log not found)"))); // no mixxx.log.1

    QString error;
    const QString path = DiagnosticReport::write(in, dir.filePath(QStringLiteral("out")), now, &error);
    ASSERT_FALSE(path.isEmpty()) << error.toStdString();
    EXPECT_TRUE(path.endsWith(QStringLiteral("BeatFreaks-report-20261106-224107.txt")));
    EXPECT_TRUE(QFile::exists(path));
}
