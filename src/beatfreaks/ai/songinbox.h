#pragma once

// Song Inbox (board task A.12): watches folders such as Downloads for new
// songs (a Suno download, an ACE-Step render, a promo) and hands each one to
// the library as soon as it is completely written. Files already in the
// folder when the inbox starts are left alone, so it never imports your
// whole Downloads folder.
//
// A file counts as ready when its size and modified time have not changed
// for `stableScans` scans in a row and it is not a browser's partial
// download (.crdownload, .part, .tmp, .download …).

#include <QDateTime>
#include <QHash>
#include <QObject>
#include <QSet>
#include <QString>
#include <QStringList>
#include <QTimer>

namespace beatfreaks::ai {

struct SongInboxSettings {
    QStringList folders;      ///< e.g. Downloads + the AI songs folder
    int scanIntervalMs = 1500;
    int stableScans = 2;      ///< unchanged scans before a file is "done"
    qint64 minBytes = 64 * 1024; ///< ignore tiny files (thumbnails, stubs)
};

class SongInbox : public QObject {
    Q_OBJECT
  public:
    explicit SongInbox(SongInboxSettings settings, QObject* parent = nullptr);

    /// Remembers what is already there, then scans every scanIntervalMs.
    void start();
    void stop();
    bool isRunning() const {
        return m_timer.isActive();
    }
    /// One scan right now (also used by tests).
    void scanNow();

    static bool isAudioFile(const QString& fileName);
    static bool isPartialDownload(const QString& fileName);
    /// "BeatFreaks AI" if a .bfai.json recipe sits next to it, else "Inbox".
    static QString sourceOf(const QString& filePath);

  signals:
    /// A new, complete song. `source` is "BeatFreaks AI" or "Inbox".
    void songArrived(const QString& filePath, const QString& source);

  private:
    struct Seen {
        qint64 size = -1;
        QDateTime modified;
        int stable = 0;
    };
    void baseline();

    SongInboxSettings m_settings;
    QTimer m_timer;
    QSet<QString> m_known;        ///< there before start, or already announced
    QHash<QString, Seen> m_pending;
};

} // namespace beatfreaks::ai
