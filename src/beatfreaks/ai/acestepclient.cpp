#include "beatfreaks/ai/acestepclient.h"

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegularExpression>
#include <QSaveFile>
#include <QTimer>
#include <QUrlQuery>
#include <QUuid>
#include <algorithm>
#include <memory>

#include "beatfreaks/diagnostics/bflog.h"

namespace beatfreaks::ai {

namespace {

QJsonValue unwrapData(const QByteArray& body) {
    const QJsonDocument doc = QJsonDocument::fromJson(body);
    if (!doc.isObject()) {
        return {};
    }
    return doc.object().value(QStringLiteral("data"));
}

QString errorText(const QByteArray& body) {
    const QJsonObject obj = QJsonDocument::fromJson(body).object();
    for (const char* key : {"error", "detail", "message"}) {
        const QJsonValue v = obj.value(QLatin1String(key));
        if (v.isString() && !v.toString().isEmpty()) {
            return v.toString();
        }
    }
    return {};
}

double toDouble(const QJsonValue& v) {
    if (v.isDouble()) {
        return v.toDouble();
    }
    bool ok = false;
    const double d = v.toString().toDouble(&ok);
    return ok ? d : 0.0;
}

QString toText(const QJsonValue& v) {
    if (v.isString()) {
        return v.toString();
    }
    if (v.isDouble()) {
        return QString::number(v.toDouble(), 'g', 12);
    }
    return {};
}

AceStepItem parseItem(const QJsonObject& o) {
    AceStepItem item;
    item.file = o.value(QStringLiteral("file")).toString();
    item.status = o.value(QStringLiteral("status")).toInt(1);
    item.prompt = o.value(QStringLiteral("prompt")).toString();
    item.lyrics = o.value(QStringLiteral("lyrics")).toString();
    item.seed = toText(o.value(QStringLiteral("seed_value")));
    item.model = o.value(QStringLiteral("dit_model")).toString();
    const QJsonObject metas = o.value(QStringLiteral("metas")).toObject();
    item.bpm = toDouble(metas.value(QStringLiteral("bpm")));
    item.keyScale = metas.value(QStringLiteral("keyscale")).toString();
    item.durationSeconds = toDouble(metas.value(QStringLiteral("duration")));
    return item;
}

} // namespace

AceStepClient::AceStepClient(AceStepSettings settings, QNetworkAccessManager* network, QObject* parent)
        : QObject(parent),
          m_settings(std::move(settings)),
          m_network(network) {
    if (!m_network) {
        m_network = new QNetworkAccessManager(this);
        m_ownsNetwork = true;
    }
}

AceStepClient::~AceStepClient() {
    const QStringList ids = m_jobs.keys();
    for (const QString& id : ids) {
        for (const QPointer<QNetworkReply>& r : std::as_const(m_jobs[id].replies)) {
            if (r) {
                r->disconnect(this);
                r->abort();
            }
        }
    }
}

// --------------------------------------------------------------- parsing --

AceStepRelease AceStepClient::parseRelease(const QByteArray& body) {
    AceStepRelease rel;
    const QJsonObject data = unwrapData(body).toObject();
    rel.taskId = data.value(QStringLiteral("task_id")).toString();
    rel.queuePosition = data.value(QStringLiteral("queue_position")).toInt(-1);
    rel.ok = !rel.taskId.isEmpty();
    if (!rel.ok) {
        rel.error = errorText(body);
        if (rel.error.isEmpty()) {
            rel.error = QStringLiteral("ACE-Step did not return a task id");
        }
    }
    return rel;
}

QList<AceStepTask> AceStepClient::parseQuery(const QByteArray& body) {
    QList<AceStepTask> tasks;
    const QJsonArray data = unwrapData(body).toArray();
    for (const QJsonValue& v : data) {
        const QJsonObject o = v.toObject();
        AceStepTask t;
        t.taskId = o.value(QStringLiteral("task_id")).toString();
        t.status = o.value(QStringLiteral("status")).toInt(0);
        // "result" is a JSON *string* holding an array (per the docs), but
        // accept a real array too.
        QJsonValue result = o.value(QStringLiteral("result"));
        if (result.isString()) {
            result = QJsonDocument::fromJson(result.toString().toUtf8()).array();
        }
        for (const QJsonValue& r : result.toArray()) {
            t.items.append(parseItem(r.toObject()));
        }
        t.error = o.value(QStringLiteral("error")).toString();
        tasks.append(t);
    }
    return tasks;
}

namespace {

int defaultPort(const QString& scheme) {
    return scheme == QLatin1String("https") ? 443 : 80;
}

bool sameOrigin(const QUrl& a, const QUrl& b) {
    const QString sa = a.scheme().toLower();
    const QString sb = b.scheme().toLower();
    if (sa != sb || (sa != QLatin1String("http") && sa != QLatin1String("https"))) {
        return false;
    }
    return a.host().compare(b.host(), Qt::CaseInsensitive) == 0 &&
            a.port(defaultPort(sa)) == b.port(defaultPort(sb));
}

} // namespace

QUrl AceStepClient::audioUrl(const QUrl& base, const QString& file) {
    if (file.isEmpty()) {
        return {};
    }
    // Resolve first, then check: "//other-host/x" or "https://…" would
    // otherwise move the download (and the API key) to another server.
    const QUrl resolved = base.resolved(QUrl(file));
    if (!resolved.isValid() || !resolved.userInfo().isEmpty() || !sameOrigin(base, resolved)) {
        return {};
    }
    return resolved;
}

bool AceStepClient::looksLikeAudio(const QByteArray& head) {
    if (head.size() < 4) {
        return false;
    }
    const auto u = [&](int i) { return static_cast<unsigned char>(head.at(i)); };
    if (head.startsWith("fLaC") || head.startsWith("ID3") || head.startsWith("RIFF") ||
            head.startsWith("OggS") || head.startsWith("FORM")) {
        return true;
    }
    if (u(0) == 0xFF && (u(1) & 0xE0) == 0xE0) {
        return true; // MPEG audio frame (MP3/AAC ADTS)
    }
    return head.size() >= 8 && head.mid(4, 4) == "ftyp"; // MP4/M4A
}

QString AceStepClient::safeFileName(const QString& name) {
    static const QRegularExpression bad(QStringLiteral(R"([\\/:*?"<>|\x00-\x1f])"));
    QString s = name;
    s.replace(bad, QStringLiteral("_"));
    s = s.simplified();
    while (s.endsWith(QLatin1Char('.')) || s.endsWith(QLatin1Char(' '))) {
        s.chop(1);
    }
    if (s.size() > 120) {
        s = s.left(120).trimmed();
    }
    return s.isEmpty() ? QStringLiteral("song") : s;
}

// ----------------------------------------------------------------- jobs ---

qint64 AceStepClient::nowMs() const {
    return QDateTime::currentMSecsSinceEpoch();
}

void AceStepClient::checkHealth() {
    QNetworkReply* reply = m_network->get(
            request(m_settings.baseUrl.resolved(QUrl(QStringLiteral("/health")))));
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) {
            emit healthChecked(false,
                    QStringLiteral("AI song server not running at %1 (%2)")
                            .arg(m_settings.baseUrl.toString(), reply->errorString()));
            return;
        }
        const QJsonObject data = unwrapData(reply->readAll()).toObject();
        const bool ok = data.value(QStringLiteral("status")).toString() == QStringLiteral("ok");
        emit healthChecked(ok,
                ok ? QStringLiteral("%1 %2").arg(data.value(QStringLiteral("service")).toString(),
                             data.value(QStringLiteral("version")).toString())
                   : QStringLiteral("AI song server answered but is not ready"));
    });
}

QString AceStepClient::generate(const SongBrief& brief) {
    const QString jobId = QUuid::createUuid().toString(QUuid::WithoutBraces).left(8);
    Job job;
    job.brief = brief;
    job.startedMs = nowMs();
    m_jobs.insert(jobId, job);
    qCInfo(bfAi) << "job" << jobId << "start:" << brief.styleText();
    if (m_settings.outputDir.isEmpty() || !QDir().mkpath(m_settings.outputDir)) {
        // report asynchronously so callers can connect after generate()
        QTimer::singleShot(0, this, [this, jobId] {
            fail(jobId, QStringLiteral("No folder to save AI songs in"));
        });
        return jobId;
    }
    QTimer::singleShot(0, this, [this, jobId] { submit(jobId); });
    return jobId;
}

void AceStepClient::cancel(const QString& jobId) {
    auto it = m_jobs.find(jobId);
    if (it == m_jobs.end()) {
        return;
    }
    for (const QPointer<QNetworkReply>& r : std::as_const(it->replies)) {
        if (r) {
            r->disconnect(this);
            r->abort();
            r->deleteLater();
        }
    }
    // remove any half-written downloads of this job
    for (const GeneratedSong& s : std::as_const(it->songs)) {
        QFile::remove(s.filePath);
        QFile::remove(s.recipePath);
    }
    m_jobs.erase(it);
    qCInfo(bfAi) << "job" << jobId << "cancelled";
    emit jobCancelled(jobId);
}

QNetworkRequest AceStepClient::request(const QUrl& url) const {
    QNetworkRequest req(url);
    req.setTransferTimeout(m_settings.requestTimeoutMs);
    // never follow a redirect to another server (it would get the API key)
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
            QNetworkRequest::SameOriginRedirectPolicy);
    if (!m_settings.apiKey.isEmpty()) {
        req.setRawHeader("Authorization", "Bearer " + m_settings.apiKey.toUtf8());
    }
    return req;
}

QNetworkReply* AceStepClient::post(const QString& jobId, const QString& path, const QJsonObject& body) {
    QNetworkRequest req = request(m_settings.baseUrl.resolved(QUrl(path)));
    req.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json"));
    QNetworkReply* reply = m_network->post(req, QJsonDocument(body).toJson(QJsonDocument::Compact));
    trackReply(jobId, reply);
    return reply;
}

QNetworkReply* AceStepClient::get(const QString& jobId, const QUrl& url) {
    QNetworkRequest req = request(url);
    req.setTransferTimeout(m_settings.requestTimeoutMs * 4); // audio files are bigger
    QNetworkReply* reply = m_network->get(req);
    trackReply(jobId, reply);
    return reply;
}

void AceStepClient::trackReply(const QString& jobId, QNetworkReply* reply) {
    auto& replies = m_jobs[jobId].replies;
    // finished replies delete themselves; forget them so polling doesn't grow
    replies.removeIf([](const QPointer<QNetworkReply>& r) { return r.isNull(); });
    replies.append(reply);
}

void AceStepClient::retryOrFail(
        const QString& jobId, void (AceStepClient::*step)(const QString&), const QString& reason) {
    auto it = m_jobs.find(jobId);
    if (it == m_jobs.end()) {
        return;
    }
    if (it->retries >= m_settings.maxRetries) {
        fail(jobId, reason);
        return;
    }
    const int delay = std::min(m_settings.maxBackoffMs,
            m_settings.pollIntervalMs * (1 << std::min(it->retries, 10)));
    ++it->retries;
    qCWarning(bfAi) << "job" << jobId << reason << "- retry" << it->retries << "in" << delay << "ms";
    QTimer::singleShot(delay, this, [this, jobId, step] {
        if (m_jobs.contains(jobId)) {
            (this->*step)(jobId);
        }
    });
}

void AceStepClient::submit(const QString& jobId) {
    if (!m_jobs.contains(jobId)) {
        return;
    }
    QNetworkReply* reply = post(jobId, QStringLiteral("/release_task"),
            m_jobs[jobId].brief.toAceStepRequest());
    connect(reply, &QNetworkReply::finished, this, [this, jobId, reply] {
        reply->deleteLater();
        if (!m_jobs.contains(jobId)) {
            return;
        }
        const int http = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        const QByteArray body = reply->readAll();
        if (http == 401) {
            fail(jobId, QStringLiteral("The AI song server wants an API key (Settings › AI songs)"));
            return;
        }
        if (http == 429 || http >= 500 || (http == 0 && reply->error() != QNetworkReply::NoError)) {
            retryOrFail(jobId, &AceStepClient::submit,
                    http == 429 ? QStringLiteral("AI song server is busy")
                                : QStringLiteral("AI song server not reachable: ") + reply->errorString());
            return;
        }
        const AceStepRelease rel = parseRelease(body);
        if (http >= 400 || !rel.ok) {
            fail(jobId, QStringLiteral("AI song server refused the song: ") +
                            (rel.error.isEmpty() ? QString::number(http) : rel.error));
            return;
        }
        Job& job = m_jobs[jobId];
        job.taskId = rel.taskId;
        job.retries = 0;
        qCInfo(bfAi) << "job" << jobId << "queued as task" << rel.taskId << "position" << rel.queuePosition;
        emit jobQueued(jobId, rel.queuePosition);
        QTimer::singleShot(m_settings.pollIntervalMs, this, [this, jobId] { poll(jobId); });
    });
}

void AceStepClient::poll(const QString& jobId) {
    if (!m_jobs.contains(jobId)) {
        return;
    }
    Job& job = m_jobs[jobId];
    const int elapsed = static_cast<int>((nowMs() - job.startedMs) / 1000);
    if (nowMs() - job.startedMs > m_settings.jobTimeoutMs) {
        fail(jobId, QStringLiteral("The AI song took too long (over %1 min)")
                            .arg(m_settings.jobTimeoutMs / 60000));
        return;
    }
    QJsonObject body;
    body[QStringLiteral("task_id_list")] = QJsonArray{job.taskId};
    QNetworkReply* reply = post(jobId, QStringLiteral("/query_result"), body);
    connect(reply, &QNetworkReply::finished, this, [this, jobId, reply, elapsed] {
        reply->deleteLater();
        if (!m_jobs.contains(jobId)) {
            return;
        }
        const int http = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (reply->error() != QNetworkReply::NoError || http >= 400) {
            retryOrFail(jobId, &AceStepClient::poll,
                    QStringLiteral("Lost the AI song server: ") + reply->errorString());
            return;
        }
        Job& job = m_jobs[jobId];
        job.retries = 0;
        const QList<AceStepTask> tasks = parseQuery(reply->readAll());
        const auto t = std::find_if(tasks.begin(), tasks.end(),
                [&](const AceStepTask& x) { return x.taskId == job.taskId; });
        if (t == tasks.end() || t->status == 0) {
            emit jobProgress(jobId, elapsed);
            QTimer::singleShot(m_settings.pollIntervalMs, this, [this, jobId] { poll(jobId); });
            return;
        }
        if (t->status == 2) {
            fail(jobId, t->error.isEmpty() ? QStringLiteral("The AI could not make this song")
                                           : t->error);
            return;
        }
        QList<AceStepItem> good;
        for (const AceStepItem& item : t->items) {
            if (item.status != 2 && audioUrl(m_settings.baseUrl, item.file).isValid()) {
                good.append(item);
            }
        }
        if (good.isEmpty()) {
            fail(jobId, QStringLiteral("The AI finished but sent no audio"));
            return;
        }
        job.toDownload = good;
        job.nextVariation = 1;
        downloadNext(jobId);
    });
}

QString AceStepClient::uniquePath(const QString& baseName, const QString& suffix) const {
    const QDir dir(m_settings.outputDir);
    QString path = dir.filePath(baseName + QLatin1Char('.') + suffix);
    for (int n = 2; QFileInfo::exists(path); ++n) {
        path = dir.filePath(QStringLiteral("%1 (%2).%3").arg(baseName).arg(n).arg(suffix));
    }
    return path;
}

void AceStepClient::downloadNext(const QString& jobId) {
    auto it = m_jobs.find(jobId);
    if (it == m_jobs.end()) {
        return;
    }
    if (it->toDownload.isEmpty()) {
        finish(jobId);
        return;
    }
    // One version at a time keeps memory, disk and the server calm.
    const AceStepItem item = it->toDownload.takeFirst();
    const int variation = it->nextVariation++;
    const QUrl url = audioUrl(m_settings.baseUrl, item.file);
    const Job& job = *it;

    QString suffix = QFileInfo(QUrlQuery(url).queryItemValue(QStringLiteral("path"), QUrl::FullyDecoded))
                             .suffix()
                             .toLower();
    static const QStringList kAudio{QStringLiteral("flac"), QStringLiteral("mp3"),
            QStringLiteral("wav"), QStringLiteral("opus"), QStringLiteral("aac"),
            QStringLiteral("ogg"), QStringLiteral("m4a")};
    if (!kAudio.contains(suffix)) {
        suffix = QStringLiteral("flac");
    }
    const QString base = safeFileName(
            QStringLiteral("BeatFreaks AI - %1 - %2 v%3").arg(job.brief.label(), jobId).arg(variation));
    const QString path = uniquePath(base, suffix);

    // Stream straight to disk. QSaveFile only appears under its real name
    // after commit(), so a cancelled or broken download leaves nothing.
    auto out = std::make_shared<QSaveFile>(path);
    if (!out->open(QIODevice::WriteOnly)) {
        qCWarning(bfAi) << "job" << jobId << "cannot write" << path;
        downloadNext(jobId);
        return;
    }
    struct Progress {
        qint64 bytes = 0;
        QByteArray head;
        bool tooBig = false;
        bool writeFailed = false;
    };
    auto progress = std::make_shared<Progress>();

    QNetworkReply* reply = get(jobId, url);
    connect(reply, &QNetworkReply::readyRead, this, [this, reply, out, progress] {
        const QByteArray chunk = reply->readAll();
        progress->bytes += chunk.size();
        if (progress->head.size() < 16) {
            progress->head += chunk.left(16 - progress->head.size());
        }
        if (progress->bytes > m_settings.maxDownloadBytes) {
            progress->tooBig = true;
            reply->abort();
            return;
        }
        if (out->write(chunk) != chunk.size()) {
            progress->writeFailed = true;
            reply->abort();
        }
    });
    connect(reply, &QNetworkReply::finished, this,
            [this, jobId, reply, item, variation, out, progress, path] {
                reply->deleteLater();
                if (!m_jobs.contains(jobId)) {
                    out->cancelWriting();
                    return;
                }
                // drain anything left (readyRead may not fire for the last bytes)
                const QByteArray rest = reply->readAll();
                if (!rest.isEmpty() && !progress->tooBig) {
                    progress->bytes += rest.size();
                    if (progress->head.size() < 16) {
                        progress->head += rest.left(16 - progress->head.size());
                    }
                    progress->tooBig = progress->bytes > m_settings.maxDownloadBytes;
                    if (!progress->tooBig && out->write(rest) != rest.size()) {
                        progress->writeFailed = true;
                    }
                }
                const int http = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
                QString problem;
                if (progress->tooBig) {
                    problem = QStringLiteral("file larger than %1 MB").arg(m_settings.maxDownloadBytes >> 20);
                } else if (progress->writeFailed) {
                    problem = QStringLiteral("disk write failed");
                } else if (reply->error() != QNetworkReply::NoError) {
                    problem = reply->errorString();
                } else if (http != 200) {
                    problem = QStringLiteral("HTTP %1").arg(http); // incl. refused redirects
                } else if (!looksLikeAudio(progress->head)) {
                    problem = QStringLiteral("not an audio file");
                }
                if (!problem.isEmpty() || !out->commit()) {
                    out->cancelWriting();
                    qCWarning(bfAi) << "job" << jobId << "version" << variation << "download failed:"
                                    << (problem.isEmpty() ? QStringLiteral("could not save") : problem);
                    downloadNext(jobId);
                    return;
                }

                Job& job = m_jobs[jobId];
                GeneratedSong song;
                song.filePath = path;
                song.variation = variation;
                song.bpm = item.bpm > 0 ? item.bpm : job.brief.bpm;
                song.keyScale = item.keyScale.isEmpty() ? job.brief.keyScale() : item.keyScale;
                song.durationSeconds = item.durationSeconds;
                song.prompt = item.prompt.isEmpty() ? job.brief.styleText() : item.prompt;
                song.lyrics = item.lyrics;
                song.seed = item.seed;
                song.model = item.model;

                // The recipe: everything needed to remake or vary this song
                // later, and how the Song Inbox knows it is AI-made.
                song.recipePath = song.filePath + QStringLiteral(".bfai.json");
                const QJsonObject recipe{
                        {QStringLiteral("source"), QStringLiteral("BeatFreaks AI (ACE-Step 1.5)")},
                        {QStringLiteral("ai_made"), true},
                        {QStringLiteral("label"), job.brief.label()},
                        {QStringLiteral("style"), song.prompt},
                        {QStringLiteral("lyrics"), song.lyrics},
                        {QStringLiteral("bpm"), song.bpm},
                        {QStringLiteral("key"), song.keyScale},
                        {QStringLiteral("camelot"), QString::fromStdString(job.brief.key.toString())},
                        {QStringLiteral("seed"), song.seed},
                        {QStringLiteral("model"), song.model},
                        {QStringLiteral("variation"), variation},
                        {QStringLiteral("task_id"), job.taskId},
                        {QStringLiteral("created"), QDateTime::currentDateTimeUtc().toString(Qt::ISODate)},
                        {QStringLiteral("request"), job.brief.toAceStepRequest()},
                };
                QSaveFile rf(song.recipePath);
                if (!rf.open(QIODevice::WriteOnly) || rf.write(QJsonDocument(recipe).toJson()) < 0 ||
                        !rf.commit()) {
                    song.recipePath.clear();
                }
                job.songs.append(song);
                qCInfo(bfAi) << "job" << jobId << "saved" << song.filePath;
                downloadNext(jobId);
            });
}

void AceStepClient::finish(const QString& jobId) {
    auto it = m_jobs.find(jobId);
    if (it == m_jobs.end()) {
        return;
    }
    const QList<GeneratedSong> songs = it->songs;
    m_jobs.erase(it);
    if (songs.isEmpty()) {
        qCWarning(bfAi) << "job" << jobId << "failed: no version could be downloaded";
        emit jobFailed(jobId, QStringLiteral("Could not download the AI song"));
        return;
    }
    qCInfo(bfAi) << "job" << jobId << "done," << songs.size() << "version(s)";
    emit jobFinished(jobId, songs);
}

void AceStepClient::fail(const QString& jobId, const QString& reason) {
    auto it = m_jobs.find(jobId);
    if (it == m_jobs.end()) {
        return;
    }
    for (const QPointer<QNetworkReply>& r : std::as_const(it->replies)) {
        if (r && r->isRunning()) {
            r->disconnect(this);
            r->abort();
            r->deleteLater();
        }
    }
    m_jobs.erase(it);
    qCWarning(bfAi) << "job" << jobId << "failed:" << reason;
    emit jobFailed(jobId, reason);
}

} // namespace beatfreaks::ai
