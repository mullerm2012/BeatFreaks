#pragma once

// Talks to a local ACE-Step 1.5 server (MIT licence, free, runs offline;
// start it with `uv run acestep-api`). API: docs/en/API.md in
// github.com/ace-step/ACE-Step-1.5.
//
//   generate(brief) ─► POST /release_task ─► task id
//                     POST /query_result every few seconds (progress)
//                     GET  /v1/audio?path=… for every version
//                     ─► songs saved in outputDir + a ".bfai.json" recipe
//
// Extras: retries with back-off when the server is busy (HTTP 429) or not
// reachable yet, a job timeout (CPU-only PCs are slow), cancel, optional
// API key, and several jobs at once.

#include <QHash>
#include <QJsonObject>
#include <QList>
#include <QObject>
#include <QPointer>
#include <QString>
#include <QUrl>

#include "beatfreaks/ai/songbrief.h"

class QNetworkAccessManager;
class QNetworkReply;

namespace beatfreaks::ai {

struct AceStepSettings {
    QUrl baseUrl = QUrl(QStringLiteral("http://127.0.0.1:8001"));
    QString apiKey;                      ///< only if the server was started with one
    QString outputDir;                   ///< where finished songs go (must be set)
    int pollIntervalMs = 2000;
    int requestTimeoutMs = 30000;        ///< one HTTP request
    int jobTimeoutMs = 30 * 60 * 1000;   ///< whole job; CPU-only can take long
    int maxRetries = 5;                  ///< per request, for busy/offline server
    int maxBackoffMs = 30000;
};

/// One finished version of a song.
struct GeneratedSong {
    QString filePath;     ///< audio on disk
    QString recipePath;   ///< ".bfai.json" next to it (prompt, lyrics, seed …)
    int variation = 1;    ///< 1-based
    double bpm = 0.0;
    QString keyScale;
    double durationSeconds = 0.0;
    QString prompt;
    QString lyrics;
    QString seed;
    QString model;
};

/// One audio item inside a finished task (parsed from /query_result).
struct AceStepItem {
    QString file;         ///< e.g. "/v1/audio?path=%2Ftmp%2Fx.flac"
    int status = 0;
    QString prompt;
    QString lyrics;
    double bpm = 0.0;
    QString keyScale;
    double durationSeconds = 0.0;
    QString seed;
    QString model;
};

struct AceStepTask {
    QString taskId;
    int status = 0;       ///< 0 queued/running, 1 done, 2 failed
    QList<AceStepItem> items;
    QString error;
};

struct AceStepRelease {
    bool ok = false;
    QString taskId;
    int queuePosition = -1;
    QString error;
};

class AceStepClient : public QObject {
    Q_OBJECT
  public:
    explicit AceStepClient(AceStepSettings settings,
            QNetworkAccessManager* network = nullptr,
            QObject* parent = nullptr);
    ~AceStepClient() override;

    const AceStepSettings& settings() const {
        return m_settings;
    }

    /// GET /health. Emits healthChecked().
    void checkHealth();
    /// Starts a job and returns its id (also used in every signal).
    QString generate(const SongBrief& brief);
    /// Stops waiting for the job and drops its results. Emits jobCancelled().
    void cancel(const QString& jobId);
    int activeJobs() const {
        return static_cast<int>(m_jobs.size());
    }

    // Parsing helpers (public for tests).
    static AceStepRelease parseRelease(const QByteArray& body);
    static QList<AceStepTask> parseQuery(const QByteArray& body);
    static QUrl audioUrl(const QUrl& base, const QString& file);
    /// File name safe on Windows: no \ / : * ? " < > |, trimmed, ≤ 120 chars.
    static QString safeFileName(const QString& name);

  signals:
    void healthChecked(bool ok, const QString& detail);
    void jobQueued(const QString& jobId, int queuePosition);
    /// Still working; elapsed time so the UI can show a timer.
    void jobProgress(const QString& jobId, int elapsedSeconds);
    void jobFinished(const QString& jobId, const QList<beatfreaks::ai::GeneratedSong>& songs);
    void jobFailed(const QString& jobId, const QString& reason);
    void jobCancelled(const QString& jobId);

  private:
    struct Job {
        SongBrief brief;
        QString taskId;
        qint64 startedMs = 0;
        int retries = 0;
        int pendingDownloads = 0;
        QList<GeneratedSong> songs;
        QList<QPointer<QNetworkReply>> replies;
    };

    void submit(const QString& jobId);
    void poll(const QString& jobId);
    void download(const QString& jobId, const AceStepItem& item, int variation);
    void retryOrFail(const QString& jobId, void (AceStepClient::*step)(const QString&),
            const QString& reason);
    void fail(const QString& jobId, const QString& reason);
    void finishIfDone(const QString& jobId);
    QNetworkReply* post(const QString& jobId, const QString& path, const QJsonObject& body);
    QNetworkReply* get(const QString& jobId, const QUrl& url);
    void trackReply(const QString& jobId, QNetworkReply* reply);
    QString uniquePath(const QString& baseName, const QString& suffix) const;
    qint64 nowMs() const;

    AceStepSettings m_settings;
    QNetworkAccessManager* m_network;
    bool m_ownsNetwork = false;
    QHash<QString, Job> m_jobs;
};

} // namespace beatfreaks::ai
