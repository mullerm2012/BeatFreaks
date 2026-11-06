#include <gtest/gtest.h>

#include <QCoreApplication>
#include <QDir>
#include <QEventLoop>
#include <QFile>
#include <QHash>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTemporaryDir>
#include <QTimer>
#include <functional>

#include "beatfreaks/ai/acestepclient.h"

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

struct Response {
    int status = 200;
    QByteArray body;
    QByteArray contentType = "application/json";
    QByteArray extraHeaders; ///< e.g. "Location: …\r\n"
};

QByteArray flac(int bytes) {
    return QByteArray("fLaC") + QByteArray(bytes - 4, 'a');
}

/// A tiny HTTP/1.1 server standing in for ACE-Step. Each route is a function
/// of the request body so a test can change answers between calls.
class FakeAceStep : public QObject {
  public:
    using Handler = std::function<Response(const QByteArray& body)>;

    FakeAceStep() {
        connect(&m_server, &QTcpServer::newConnection, this, [this] {
            while (QTcpSocket* s = m_server.nextPendingConnection()) {
                connect(s, &QTcpSocket::readyRead, this, [this, s] { onData(s); });
                connect(s, &QTcpSocket::disconnected, s, &QObject::deleteLater);
            }
        });
        m_server.listen(QHostAddress::LocalHost, 0);
    }
    QUrl url() const {
        return QUrl(QStringLiteral("http://127.0.0.1:%1").arg(m_server.serverPort()));
    }
    void on(const QByteArray& method, const QByteArray& path, Handler h) {
        m_routes[method + ' ' + path] = std::move(h);
    }
    int hits(const QByteArray& path) const {
        return m_hits.value(path);
    }
    QByteArray lastAuth;
    QByteArray lastReleaseBody;

  private:
    void onData(QTcpSocket* s) {
        QByteArray& buf = m_buffers[s];
        buf += s->readAll();
        const int headerEnd = buf.indexOf("\r\n\r\n");
        if (headerEnd < 0) {
            return;
        }
        const QList<QByteArray> lines = buf.left(headerEnd).split('\n');
        const QList<QByteArray> first = lines.value(0).trimmed().split(' ');
        qsizetype length = 0;
        QByteArray auth;
        for (const QByteArray& l : lines) {
            const QByteArray lower = l.trimmed().toLower();
            if (lower.startsWith("content-length:")) {
                length = l.mid(l.indexOf(':') + 1).trimmed().toLongLong();
            } else if (lower.startsWith("authorization:")) {
                auth = l.mid(l.indexOf(':') + 1).trimmed();
            }
        }
        if (buf.size() < headerEnd + 4 + length) {
            return;
        }
        const QByteArray body = buf.mid(headerEnd + 4, length);
        m_buffers.remove(s);
        const QByteArray method = first.value(0);
        const QByteArray path = first.value(1).split('?').value(0);
        ++m_hits[path];
        lastAuth = auth;
        if (path == "/release_task") {
            lastReleaseBody = body;
        }
        const auto it = m_routes.constFind(method + ' ' + path);
        const Response r = it != m_routes.cend() ? (*it)(body) : Response{404, "{\"detail\":\"no\"}"};
        QByteArray out = "HTTP/1.1 " + QByteArray::number(r.status) + " X\r\nContent-Type: " +
                r.contentType + "\r\nContent-Length: " + QByteArray::number(r.body.size()) +
                "\r\n" + r.extraHeaders + "Connection: close\r\n\r\n" + r.body;
        s->write(out);
        s->flush();
        s->disconnectFromHost();
    }

    QTcpServer m_server;
    QHash<QByteArray, Handler> m_routes;
    QHash<QTcpSocket*, QByteArray> m_buffers;
    QHash<QByteArray, int> m_hits;
};

QByteArray wrap(const QJsonValue& data) {
    QJsonObject o{{"data", data}, {"code", 200}, {"error", QJsonValue()}};
    return QJsonDocument(o).toJson(QJsonDocument::Compact);
}

QByteArray released(const QString& id) {
    return wrap(QJsonObject{{"task_id", id}, {"status", "queued"}, {"queue_position", 1}});
}

QByteArray queryAnswer(const QString& id, int status, int items = 2) {
    QJsonArray result;
    for (int i = 0; i < items; ++i) {
        result.append(QJsonObject{
                {"file", QString(QStringLiteral("/v1/audio?path=%2Ftmp%2Fapi_audio%2Fsong") +
                                 QString::number(i) + QStringLiteral(".flac"))},
                {"status", 1},
                {"prompt", "tech house"},
                {"lyrics", "[Intro]"},
                {"seed_value", QString::number(1000 + i)},
                {"dit_model", "acestep-v15-turbo"},
                {"metas", QJsonObject{{"bpm", 128}, {"duration", 240}, {"keyscale", "A minor"}}},
        });
    }
    QJsonObject task{{"task_id", id}, {"status", status}};
    if (status == 1) {
        task["result"] = QString::fromUtf8(QJsonDocument(result).toJson(QJsonDocument::Compact));
    }
    if (status == 2) {
        task["error"] = "CUDA out of memory";
    }
    return wrap(QJsonArray{task});
}

/// Runs the event loop until `done` is true or the timeout expires.
bool waitFor(const std::function<bool()>& done, int timeoutMs = 10000) {
    QEventLoop loop;
    QTimer tick;
    QTimer limit;
    QObject::connect(&tick, &QTimer::timeout, &loop, [&] {
        if (done()) {
            loop.quit();
        }
    });
    QObject::connect(&limit, &QTimer::timeout, &loop, &QEventLoop::quit);
    tick.start(5);
    limit.setSingleShot(true);
    limit.start(timeoutMs);
    if (!done()) {
        loop.exec();
    }
    return done();
}

struct Outcome {
    bool finished = false;
    bool failed = false;
    bool cancelled = false;
    int queued = -2;
    int progress = 0;
    QString reason;
    QList<GeneratedSong> songs;
};

void watch(AceStepClient& c, Outcome& o) {
    QObject::connect(&c, &AceStepClient::jobQueued, [&o](const QString&, int p) { o.queued = p; });
    QObject::connect(&c, &AceStepClient::jobProgress, [&o](const QString&, int) { ++o.progress; });
    QObject::connect(&c, &AceStepClient::jobFinished,
            [&o](const QString&, const QList<GeneratedSong>& s) {
                o.finished = true;
                o.songs = s;
            });
    QObject::connect(&c, &AceStepClient::jobFailed, [&o](const QString&, const QString& r) {
        o.failed = true;
        o.reason = r;
    });
    QObject::connect(&c, &AceStepClient::jobCancelled, [&o](const QString&) { o.cancelled = true; });
}

AceStepSettings fastSettings(const QUrl& url, const QString& dir) {
    AceStepSettings s;
    s.baseUrl = url;
    s.outputDir = dir;
    s.pollIntervalMs = 10;
    s.maxBackoffMs = 50;
    s.maxRetries = 3;
    s.requestTimeoutMs = 3000;
    return s;
}

SongBrief brief() {
    SongBrief b;
    b.genre = QStringLiteral("tech house");
    b.bpm = 128;
    b.key = beatfreaks::CamelotKey::parse("8A");
    return b;
}

class AceStepClientTest : public ::testing::Test {
  protected:
    void SetUp() override {
        ensureApp();
        ASSERT_TRUE(m_dir.isValid());
    }
    QTemporaryDir m_dir;
};

} // namespace

// ------------------------------------------------------------ parsing -----

TEST(AceStepParse, Release) {
    const AceStepRelease ok = AceStepClient::parseRelease(released("abc"));
    EXPECT_TRUE(ok.ok);
    EXPECT_EQ(ok.taskId, QStringLiteral("abc"));
    EXPECT_EQ(ok.queuePosition, 1);
    const AceStepRelease bad = AceStepClient::parseRelease("{\"detail\":\"bad json\"}");
    EXPECT_FALSE(bad.ok);
    EXPECT_EQ(bad.error, QStringLiteral("bad json"));
    EXPECT_FALSE(AceStepClient::parseRelease("not json").ok);
}

TEST(AceStepParse, QueryWithResultAsStringOrArray) {
    const QList<AceStepTask> t = AceStepClient::parseQuery(queryAnswer("t1", 1, 2));
    ASSERT_EQ(t.size(), 1);
    EXPECT_EQ(t[0].status, 1);
    ASSERT_EQ(t[0].items.size(), 2);
    EXPECT_DOUBLE_EQ(t[0].items[0].bpm, 128.0);
    EXPECT_EQ(t[0].items[1].seed, QStringLiteral("1001"));
    EXPECT_EQ(t[0].items[0].keyScale, QStringLiteral("A minor"));

    const QByteArray asArray = wrap(QJsonArray{QJsonObject{{"task_id", "t2"}, {"status", 1},
            {"result", QJsonArray{QJsonObject{{"file", "/v1/audio?path=x.mp3"}}}}}});
    const QList<AceStepTask> t2 = AceStepClient::parseQuery(asArray);
    ASSERT_EQ(t2.size(), 1);
    ASSERT_EQ(t2[0].items.size(), 1);
    EXPECT_EQ(t2[0].items[0].file, QStringLiteral("/v1/audio?path=x.mp3"));

    EXPECT_TRUE(AceStepClient::parseQuery("garbage").isEmpty());
    EXPECT_EQ(AceStepClient::parseQuery(queryAnswer("t3", 2))[0].error,
            QStringLiteral("CUDA out of memory"));
}

TEST(AceStepParse, AudioUrlOnlyFromTheServer) {
    const QUrl base(QStringLiteral("http://127.0.0.1:8001"));
    // DATA's review: check the *resolved* URL, not the raw string
    EXPECT_FALSE(AceStepClient::audioUrl(base, QStringLiteral("//evil.invalid/audio")).isValid());
    EXPECT_FALSE(AceStepClient::audioUrl(base, QStringLiteral("https://127.0.0.1:8001/v1/audio")).isValid());
    EXPECT_FALSE(AceStepClient::audioUrl(base, QStringLiteral("http://127.0.0.1:9999/v1/audio")).isValid());
    EXPECT_FALSE(AceStepClient::audioUrl(base, QStringLiteral("http://me:pw@127.0.0.1:8001/v1/audio")).isValid());
    EXPECT_FALSE(AceStepClient::audioUrl(base, QStringLiteral("file:///etc/passwd")).isValid());
    EXPECT_FALSE(AceStepClient::audioUrl(QUrl(QStringLiteral("ftp://127.0.0.1")), QStringLiteral("/x")).isValid());
    EXPECT_TRUE(AceStepClient::audioUrl(QUrl(QStringLiteral("http://Localhost:80")),
            QStringLiteral("http://localhost/v1/audio?path=a")).isValid()); // default port, host case
    EXPECT_EQ(AceStepClient::audioUrl(base, QStringLiteral("/v1/audio?path=%2Ftmp%2Fa.flac")).toString(),
            QStringLiteral("http://127.0.0.1:8001/v1/audio?path=%2Ftmp%2Fa.flac"));
    EXPECT_TRUE(AceStepClient::audioUrl(base, QStringLiteral("http://127.0.0.1:8001/v1/audio?path=a"))
                    .isValid());
    EXPECT_FALSE(AceStepClient::audioUrl(base, QStringLiteral("http://evil.example/a.flac")).isValid());
    EXPECT_FALSE(AceStepClient::audioUrl(base, QString()).isValid());
}

TEST(AceStepParse, LooksLikeAudio) {
    EXPECT_TRUE(AceStepClient::looksLikeAudio("fLaC\0\0\0\x22"));
    EXPECT_TRUE(AceStepClient::looksLikeAudio("ID3\x04\0\0"));
    EXPECT_TRUE(AceStepClient::looksLikeAudio(QByteArray("\xFF\xFB\x90\x00", 4)));
    EXPECT_TRUE(AceStepClient::looksLikeAudio("RIFF\0\0\0\0WAVE"));
    EXPECT_TRUE(AceStepClient::looksLikeAudio("OggS\0\x02"));
    EXPECT_TRUE(AceStepClient::looksLikeAudio(QByteArray("\0\0\0 ftypM4A ", 12)));
    EXPECT_FALSE(AceStepClient::looksLikeAudio("<html><body>"));
    EXPECT_FALSE(AceStepClient::looksLikeAudio("{\"detail\":1}"));
    EXPECT_FALSE(AceStepClient::looksLikeAudio("fL"));
}

TEST(AceStepParse, SafeFileName) {
    EXPECT_EQ(AceStepClient::safeFileName(QStringLiteral("a/b:c*d?\"e<f>g|h")),
            QStringLiteral("a_b_c_d__e_f_g_h"));
    EXPECT_EQ(AceStepClient::safeFileName(QStringLiteral("  trailing.  ")), QStringLiteral("trailing"));
    EXPECT_EQ(AceStepClient::safeFileName(QString()), QStringLiteral("song"));
    EXPECT_LE(AceStepClient::safeFileName(QString(300, QLatin1Char('x'))).size(), 120);
}

// ------------------------------------------------------------ end to end --

TEST_F(AceStepClientTest, MakesSongsAndSavesRecipes) {
    FakeAceStep server;
    int polls = 0;
    server.on("POST", "/release_task", [](const QByteArray&) { return Response{200, released("t1")}; });
    server.on("POST", "/query_result", [&polls](const QByteArray&) {
        return Response{200, queryAnswer("t1", ++polls < 3 ? 0 : 1, 2)};
    });
    server.on("GET", "/v1/audio", [](const QByteArray&) {
        return Response{200, flac(4096), "audio/flac"};
    });
    AceStepSettings s = fastSettings(server.url(), m_dir.path());
    s.apiKey = QStringLiteral("secret");
    AceStepClient client(s);
    Outcome o;
    watch(client, o);
    client.generate(brief());
    ASSERT_TRUE(waitFor([&] { return o.finished || o.failed; })) << "timed out";
    ASSERT_TRUE(o.finished) << o.reason.toStdString();
    EXPECT_EQ(o.queued, 1);
    EXPECT_GE(o.progress, 2);
    EXPECT_EQ(server.lastAuth, QByteArray("Bearer secret"));
    EXPECT_TRUE(server.lastReleaseBody.contains("\"key_scale\":\"A minor\""));
    ASSERT_EQ(o.songs.size(), 2);
    EXPECT_EQ(o.songs[0].variation, 1);
    EXPECT_EQ(o.songs[1].seed, QStringLiteral("1001"));
    for (const GeneratedSong& song : o.songs) {
        EXPECT_TRUE(song.filePath.endsWith(QStringLiteral(".flac")));
        EXPECT_TRUE(song.filePath.contains(QStringLiteral("BeatFreaks AI - tech house 128 8A")));
        QFile f(song.filePath);
        ASSERT_TRUE(f.open(QIODevice::ReadOnly));
        EXPECT_EQ(f.size(), 4096);
        QFile r(song.recipePath);
        ASSERT_TRUE(r.open(QIODevice::ReadOnly));
        const QJsonObject recipe = QJsonDocument::fromJson(r.readAll()).object();
        EXPECT_TRUE(recipe.value(QStringLiteral("ai_made")).toBool());
        EXPECT_EQ(recipe.value(QStringLiteral("camelot")).toString(), QStringLiteral("8A"));
        EXPECT_EQ(recipe.value(QStringLiteral("seed")).toString(), song.seed);
    }
    EXPECT_EQ(client.activeJobs(), 0);
}

TEST_F(AceStepClientTest, RetriesWhenServerIsBusy) {
    FakeAceStep server;
    int calls = 0;
    server.on("POST", "/release_task", [&calls](const QByteArray&) {
        return ++calls < 3 ? Response{429, "{\"detail\":\"busy\"}"} : Response{200, released("t9")};
    });
    server.on("POST", "/query_result", [](const QByteArray&) { return Response{200, queryAnswer("t9", 1, 1)}; });
    server.on("GET", "/v1/audio", [](const QByteArray&) { return Response{200, "RIFF....", "audio/flac"}; });
    AceStepClient client(fastSettings(server.url(), m_dir.path()));
    Outcome o;
    watch(client, o);
    client.generate(brief());
    ASSERT_TRUE(waitFor([&] { return o.finished || o.failed; }));
    EXPECT_TRUE(o.finished) << o.reason.toStdString();
    EXPECT_EQ(calls, 3);
}

TEST_F(AceStepClientTest, GivesUpAfterMaxRetries) {
    FakeAceStep server;
    server.on("POST", "/release_task", [](const QByteArray&) { return Response{429, "{}"}; });
    AceStepClient client(fastSettings(server.url(), m_dir.path()));
    Outcome o;
    watch(client, o);
    client.generate(brief());
    ASSERT_TRUE(waitFor([&] { return o.failed || o.finished; }));
    EXPECT_TRUE(o.failed);
    EXPECT_TRUE(o.reason.contains(QStringLiteral("busy")));
    EXPECT_EQ(server.hits("/release_task"), 4); // first try + 3 retries
}

TEST_F(AceStepClientTest, AskForApiKeyOn401) {
    FakeAceStep server;
    server.on("POST", "/release_task", [](const QByteArray&) { return Response{401, "{}"}; });
    AceStepClient client(fastSettings(server.url(), m_dir.path()));
    Outcome o;
    watch(client, o);
    client.generate(brief());
    ASSERT_TRUE(waitFor([&] { return o.failed; }));
    EXPECT_TRUE(o.reason.contains(QStringLiteral("API key")));
    EXPECT_EQ(server.hits("/release_task"), 1);
}

TEST_F(AceStepClientTest, ReportsModelFailure) {
    FakeAceStep server;
    server.on("POST", "/release_task", [](const QByteArray&) { return Response{200, released("t2")}; });
    server.on("POST", "/query_result", [](const QByteArray&) { return Response{200, queryAnswer("t2", 2)}; });
    AceStepClient client(fastSettings(server.url(), m_dir.path()));
    Outcome o;
    watch(client, o);
    client.generate(brief());
    ASSERT_TRUE(waitFor([&] { return o.failed; }));
    EXPECT_EQ(o.reason, QStringLiteral("CUDA out of memory"));
}

TEST_F(AceStepClientTest, CancelStopsTheJob) {
    FakeAceStep server;
    server.on("POST", "/release_task", [](const QByteArray&) { return Response{200, released("t3")}; });
    server.on("POST", "/query_result", [](const QByteArray&) { return Response{200, queryAnswer("t3", 0)}; });
    AceStepClient client(fastSettings(server.url(), m_dir.path()));
    Outcome o;
    watch(client, o);
    const QString id = client.generate(brief());
    ASSERT_TRUE(waitFor([&] { return o.queued == 1; }));
    client.cancel(id);
    EXPECT_TRUE(o.cancelled);
    EXPECT_EQ(client.activeJobs(), 0);
    const int polls = server.hits("/query_result");
    waitFor([] { return false; }, 150);
    EXPECT_LE(server.hits("/query_result"), polls + 1); // at most one in flight
    EXPECT_FALSE(o.finished);
    EXPECT_FALSE(o.failed);
}

TEST_F(AceStepClientTest, TimesOutSlowJobs) {
    FakeAceStep server;
    server.on("POST", "/release_task", [](const QByteArray&) { return Response{200, released("t4")}; });
    server.on("POST", "/query_result", [](const QByteArray&) { return Response{200, queryAnswer("t4", 0)}; });
    AceStepSettings s = fastSettings(server.url(), m_dir.path());
    s.jobTimeoutMs = 200;
    AceStepClient client(s);
    Outcome o;
    watch(client, o);
    client.generate(brief());
    ASSERT_TRUE(waitFor([&] { return o.failed; }));
    EXPECT_TRUE(o.reason.contains(QStringLiteral("too long")));
}

TEST_F(AceStepClientTest, FailsWithoutOutputFolder) {
    AceStepClient client(fastSettings(QUrl(QStringLiteral("http://127.0.0.1:1")), QString()));
    Outcome o;
    watch(client, o);
    client.generate(brief());
    ASSERT_TRUE(waitFor([&] { return o.failed; }));
    EXPECT_TRUE(o.reason.contains(QStringLiteral("folder")));
}

TEST_F(AceStepClientTest, ServerNotRunning) {
    quint16 freePort = 0;
    {
        QTcpServer probe;
        probe.listen(QHostAddress::LocalHost, 0);
        freePort = probe.serverPort();
    }
    AceStepSettings s = fastSettings(QUrl(QStringLiteral("http://127.0.0.1:%1").arg(freePort)), m_dir.path());
    s.maxRetries = 1;
    AceStepClient client(s);
    Outcome o;
    watch(client, o);
    bool healthDone = false;
    bool healthy = true;
    QObject::connect(&client, &AceStepClient::healthChecked, [&](bool ok, const QString&) {
        healthDone = true;
        healthy = ok;
    });
    client.checkHealth();
    client.generate(brief());
    ASSERT_TRUE(waitFor([&] { return o.failed && healthDone; }));
    EXPECT_FALSE(healthy);
    EXPECT_TRUE(o.reason.contains(QStringLiteral("not reachable")));
}

TEST_F(AceStepClientTest, HealthCheckOk) {
    FakeAceStep server;
    server.on("GET", "/health", [](const QByteArray&) {
        return Response{200, wrap(QJsonObject{{"status", "ok"}, {"service", "ACE-Step API"}, {"version", "1.0"}})};
    });
    AceStepClient client(fastSettings(server.url(), m_dir.path()));
    bool done = false;
    bool healthy = false;
    QString detail;
    QObject::connect(&client, &AceStepClient::healthChecked, [&](bool ok, const QString& d) {
        done = true;
        healthy = ok;
        detail = d;
    });
    client.checkHealth();
    ASSERT_TRUE(waitFor([&] { return done; }));
    EXPECT_TRUE(healthy);
    EXPECT_EQ(detail, QStringLiteral("ACE-Step API 1.0"));
}

// --------------------------------------------- download safety (DATA) ---

namespace {

/// Serves release + a finished task whose files are /v1/audio?path=song0..N.
void serveFinishedTask(FakeAceStep& server, int items) {
    server.on("POST", "/release_task", [](const QByteArray&) { return Response{200, released("t7")}; });
    server.on("POST", "/query_result", [items](const QByteArray&) {
        return Response{200, queryAnswer("t7", 1, items)};
    });
}

} // namespace

TEST_F(AceStepClientTest, RefusesRedirectToAnotherServer) {
    FakeAceStep evil;
    evil.on("GET", "/steal", [](const QByteArray&) { return Response{200, flac(4096), "audio/flac"}; });
    FakeAceStep server;
    serveFinishedTask(server, 1);
    const QByteArray location = "Location: " + evil.url().toString().toUtf8() + "/steal\r\n";
    server.on("GET", "/v1/audio", [location](const QByteArray&) {
        return Response{302, "", "text/plain", location};
    });
    AceStepSettings s = fastSettings(server.url(), m_dir.path());
    s.apiKey = QStringLiteral("secret");
    AceStepClient client(s);
    Outcome o;
    watch(client, o);
    client.generate(brief());
    ASSERT_TRUE(waitFor([&] { return o.failed || o.finished; }));
    EXPECT_TRUE(o.failed);
    EXPECT_EQ(evil.hits("/steal"), 0); // the key never left
    EXPECT_TRUE(QDir(m_dir.path()).entryList(QDir::Files).isEmpty());
}

TEST_F(AceStepClientTest, RejectsNonAudioAndErrors) {
    FakeAceStep server;
    serveFinishedTask(server, 3);
    int n = 0;
    server.on("GET", "/v1/audio", [&n](const QByteArray&) {
        switch (n++) {
        case 0:
            return Response{200, "<html>login page</html>" + QByteArray(2000, ' '), "text/html"};
        case 1:
            return Response{404, "{\"detail\":\"gone\"}"};
        default:
            return Response{200, flac(3000), "audio/flac"};
        }
    });
    AceStepClient client(fastSettings(server.url(), m_dir.path()));
    Outcome o;
    watch(client, o);
    client.generate(brief());
    ASSERT_TRUE(waitFor([&] { return o.failed || o.finished; }));
    ASSERT_TRUE(o.finished) << o.reason.toStdString();
    ASSERT_EQ(o.songs.size(), 1); // only the real audio file is kept
    EXPECT_EQ(o.songs[0].variation, 3);
    // no leftovers from the two bad versions: one song + its recipe
    EXPECT_EQ(QDir(m_dir.path()).entryList(QDir::Files).size(), 2);
}

TEST_F(AceStepClientTest, CapsDownloadSize) {
    FakeAceStep server;
    serveFinishedTask(server, 1);
    server.on("GET", "/v1/audio", [](const QByteArray&) { return Response{200, flac(200000), "audio/flac"}; });
    AceStepSettings s = fastSettings(server.url(), m_dir.path());
    s.maxDownloadBytes = 50000;
    AceStepClient client(s);
    Outcome o;
    watch(client, o);
    client.generate(brief());
    ASSERT_TRUE(waitFor([&] { return o.failed || o.finished; }));
    EXPECT_TRUE(o.failed);
    EXPECT_TRUE(QDir(m_dir.path()).entryList(QDir::Files).isEmpty());
}

TEST_F(AceStepClientTest, DownloadsOneVersionAtATime) {
    FakeAceStep server;
    serveFinishedTask(server, 4);
    server.on("GET", "/v1/audio", [](const QByteArray&) { return Response{200, flac(1000), "audio/flac"}; });
    AceStepClient client(fastSettings(server.url(), m_dir.path()));
    Outcome o;
    watch(client, o);
    client.generate(brief());
    ASSERT_TRUE(waitFor([&] { return o.failed || o.finished; }));
    ASSERT_TRUE(o.finished);
    ASSERT_EQ(o.songs.size(), 4);
    for (int i = 0; i < 4; ++i) {
        EXPECT_EQ(o.songs[i].variation, i + 1);
    }
    EXPECT_EQ(server.hits("/v1/audio"), 4);
}

TEST_F(AceStepClientTest, RefusesHugeStatusReplies) {
    FakeAceStep server;
    server.on("POST", "/release_task", [](const QByteArray&) { return Response{200, released("t8")}; });
    server.on("POST", "/query_result", [](const QByteArray&) {
        return Response{200, QByteArray(3 * 1024 * 1024, ' ')};
    });
    AceStepSettings s = fastSettings(server.url(), m_dir.path());
    s.maxRetries = 1;
    AceStepClient client(s);
    Outcome o;
    watch(client, o);
    client.generate(brief());
    ASSERT_TRUE(waitFor([&] { return o.failed || o.finished; }));
    EXPECT_TRUE(o.failed);
}
