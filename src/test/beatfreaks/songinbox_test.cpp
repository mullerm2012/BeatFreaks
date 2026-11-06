#include <gtest/gtest.h>

#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QTemporaryDir>

#include "beatfreaks/ai/songinbox.h"

using namespace beatfreaks::ai;

namespace {

void ensureApp() {
    if (!QCoreApplication::instance()) {
        static int argc = 1;
        static char name[] = "beatfreaks_core_tests";
        static char* argv[] = {name, nullptr};
        static QCoreApplication app(argc, argv);
    }
}

void writeFile(const QString& path, qint64 bytes) {
    QFile f(path);
    ASSERT_TRUE(f.open(QIODevice::WriteOnly));
    f.write(QByteArray(bytes, 'x'));
}

class SongInboxTest : public ::testing::Test {
  protected:
    void SetUp() override {
        ensureApp();
        ASSERT_TRUE(m_dir.isValid());
        m_settings.folders = {m_dir.path()};
        m_settings.minBytes = 100;
        m_settings.stableScans = 2;
    }
    QString path(const char* name) const {
        return QDir(m_dir.path()).filePath(QString::fromLatin1(name));
    }
    QTemporaryDir m_dir;
    SongInboxSettings m_settings;
};

} // namespace

TEST(SongInboxNames, AudioAndPartialDownloads) {
    EXPECT_TRUE(SongInbox::isAudioFile(QStringLiteral("Freaks.MP3")));
    EXPECT_TRUE(SongInbox::isAudioFile(QStringLiteral("a.flac")));
    EXPECT_TRUE(SongInbox::isAudioFile(QStringLiteral("a.wav")));
    EXPECT_FALSE(SongInbox::isAudioFile(QStringLiteral("cover.jpg")));
    EXPECT_FALSE(SongInbox::isAudioFile(QStringLiteral("song.mp3.crdownload")));
    EXPECT_FALSE(SongInbox::isAudioFile(QStringLiteral("song.mp3.part")));
    EXPECT_FALSE(SongInbox::isAudioFile(QStringLiteral(".hidden.mp3")));
    EXPECT_TRUE(SongInbox::isPartialDownload(QStringLiteral("x.tmp")));
    EXPECT_FALSE(SongInbox::isPartialDownload(QStringLiteral("x.mp3")));
}

TEST_F(SongInboxTest, LeavesExistingSongsAlone) {
    writeFile(path("old.mp3"), 1000);
    SongInbox inbox(m_settings);
    QStringList arrived;
    QObject::connect(&inbox, &SongInbox::songArrived,
            [&](const QString& p, const QString&) { arrived << p; });
    inbox.start();
    for (int i = 0; i < 4; ++i) {
        inbox.scanNow();
    }
    EXPECT_TRUE(arrived.isEmpty());
    inbox.stop();
    EXPECT_FALSE(inbox.isRunning());
}

TEST_F(SongInboxTest, AnnouncesANewSongOnceItStopsGrowing) {
    SongInbox inbox(m_settings);
    QStringList arrived;
    QStringList sources;
    QObject::connect(&inbox, &SongInbox::songArrived, [&](const QString& p, const QString& s) {
        arrived << p;
        sources << s;
    });
    inbox.start();
    writeFile(path("new.mp3"), 500);
    inbox.scanNow(); // first sight
    writeFile(path("new.mp3"), 900); // still downloading
    inbox.scanNow();
    inbox.scanNow();
    EXPECT_TRUE(arrived.isEmpty());
    inbox.scanNow(); // unchanged twice → done
    ASSERT_EQ(arrived.size(), 1);
    EXPECT_TRUE(arrived[0].endsWith(QStringLiteral("new.mp3")));
    EXPECT_EQ(sources[0], QStringLiteral("Inbox"));
    inbox.scanNow();
    inbox.scanNow();
    EXPECT_EQ(arrived.size(), 1); // only once
}

TEST_F(SongInboxTest, BrowserPartialThenRename) {
    SongInbox inbox(m_settings);
    QStringList arrived;
    QObject::connect(&inbox, &SongInbox::songArrived,
            [&](const QString& p, const QString&) { arrived << p; });
    inbox.start();
    writeFile(path("suno.mp3.crdownload"), 5000);
    for (int i = 0; i < 4; ++i) {
        inbox.scanNow();
    }
    EXPECT_TRUE(arrived.isEmpty());
    ASSERT_TRUE(QFile::rename(path("suno.mp3.crdownload"), path("suno.mp3")));
    for (int i = 0; i < 3; ++i) {
        inbox.scanNow();
    }
    ASSERT_EQ(arrived.size(), 1);
    EXPECT_TRUE(arrived[0].endsWith(QStringLiteral("suno.mp3")));
}

TEST_F(SongInboxTest, IgnoresTinyFiles) {
    SongInbox inbox(m_settings);
    int count = 0;
    QObject::connect(&inbox, &SongInbox::songArrived, [&](const QString&, const QString&) { ++count; });
    inbox.start();
    writeFile(path("stub.mp3"), 10);
    for (int i = 0; i < 4; ++i) {
        inbox.scanNow();
    }
    EXPECT_EQ(count, 0);
}

TEST_F(SongInboxTest, KnowsAiMadeSongsByTheirRecipe) {
    SongInbox inbox(m_settings);
    QString source;
    QObject::connect(&inbox, &SongInbox::songArrived, [&](const QString&, const QString& s) { source = s; });
    inbox.start();
    writeFile(path("ai.flac"), 2000);
    writeFile(path("ai.flac.bfai.json"), 10);
    for (int i = 0; i < 3; ++i) {
        inbox.scanNow();
    }
    EXPECT_EQ(source, QStringLiteral("BeatFreaks AI"));
}

TEST_F(SongInboxTest, ForgetsFilesDeletedBeforeFinishing) {
    SongInbox inbox(m_settings);
    int count = 0;
    QObject::connect(&inbox, &SongInbox::songArrived, [&](const QString&, const QString&) { ++count; });
    inbox.start();
    writeFile(path("gone.mp3"), 2000);
    inbox.scanNow();
    ASSERT_TRUE(QFile::remove(path("gone.mp3")));
    inbox.scanNow();
    writeFile(path("gone.mp3"), 3000);
    inbox.scanNow(); // starts counting again from zero
    inbox.scanNow();
    EXPECT_EQ(count, 0);
    inbox.scanNow();
    EXPECT_EQ(count, 1);
}
