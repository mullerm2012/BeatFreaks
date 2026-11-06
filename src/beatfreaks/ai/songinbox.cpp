#include "beatfreaks/ai/songinbox.h"

#include <QDir>
#include <QFileInfo>

#include "beatfreaks/diagnostics/bflog.h"
#include "moc_songinbox.cpp"

namespace beatfreaks::ai {

namespace {

const QStringList& audioSuffixes() {
    static const QStringList s{QStringLiteral("mp3"), QStringLiteral("wav"),
            QStringLiteral("flac"), QStringLiteral("aif"), QStringLiteral("aiff"),
            QStringLiteral("m4a"), QStringLiteral("aac"), QStringLiteral("ogg"),
            QStringLiteral("opus"), QStringLiteral("wv")};
    return s;
}

QFileInfoList audioFilesIn(const QString& folder) {
    QDir dir(folder);
    QFileInfoList out;
    const QFileInfoList all = dir.entryInfoList(QDir::Files | QDir::Readable | QDir::NoDotAndDotDot);
    for (const QFileInfo& fi : all) {
        if (SongInbox::isAudioFile(fi.fileName()) && !SongInbox::isPartialDownload(fi.fileName())) {
            out.append(fi);
        }
    }
    return out;
}

} // namespace

SongInbox::SongInbox(SongInboxSettings settings, QObject* parent)
        : QObject(parent),
          m_settings(std::move(settings)) {
    m_timer.setInterval(m_settings.scanIntervalMs);
    connect(&m_timer, &QTimer::timeout, this, &SongInbox::scanNow);
}

bool SongInbox::isPartialDownload(const QString& fileName) {
    const QString n = fileName.toLower();
    for (const char* s : {".crdownload", ".part", ".partial", ".tmp", ".download", ".opdownload"}) {
        if (n.endsWith(QLatin1String(s))) {
            return true;
        }
    }
    return n.startsWith(QLatin1Char('.')) || n.startsWith(QStringLiteral("~$"));
}

bool SongInbox::isAudioFile(const QString& fileName) {
    if (isPartialDownload(fileName)) {
        return false;
    }
    return audioSuffixes().contains(QFileInfo(fileName).suffix().toLower());
}

QString SongInbox::sourceOf(const QString& filePath) {
    return QFileInfo::exists(filePath + QStringLiteral(".bfai.json"))
            ? QStringLiteral("BeatFreaks AI")
            : QStringLiteral("Inbox");
}

void SongInbox::baseline() {
    m_known.clear();
    m_pending.clear();
    for (const QString& folder : std::as_const(m_settings.folders)) {
        for (const QFileInfo& fi : audioFilesIn(folder)) {
            m_known.insert(fi.absoluteFilePath());
        }
    }
}

void SongInbox::start() {
    baseline();
    qCInfo(bfAi) << "Song Inbox watching" << m_settings.folders << "(" << m_known.size()
                 << "existing songs left alone)";
    m_timer.start();
}

void SongInbox::stop() {
    m_timer.stop();
}

void SongInbox::scanNow() {
    QSet<QString> present;
    for (const QString& folder : std::as_const(m_settings.folders)) {
        for (const QFileInfo& fi : audioFilesIn(folder)) {
            const QString path = fi.absoluteFilePath();
            present.insert(path);
            if (m_known.contains(path)) {
                continue;
            }
            Seen& seen = m_pending[path];
            const qint64 size = fi.size();
            const QDateTime modified = fi.lastModified();
            if (size == seen.size && modified == seen.modified) {
                ++seen.stable;
            } else {
                seen.size = size;
                seen.modified = modified;
                seen.stable = 0;
            }
            if (seen.stable >= m_settings.stableScans && size >= m_settings.minBytes) {
                m_pending.remove(path);
                m_known.insert(path);
                const QString source = sourceOf(path);
                qCInfo(bfAi) << "Song Inbox: new song" << path << "from" << source;
                emit songArrived(path, source);
            }
        }
    }
    // forget files that disappeared (renamed or deleted before finishing)
    for (auto it = m_pending.begin(); it != m_pending.end();) {
        it = present.contains(it.key()) ? std::next(it) : m_pending.erase(it);
    }
}

} // namespace beatfreaks::ai
