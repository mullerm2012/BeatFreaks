#include "beatfreaks/library/tracklist.h"

#include <QFileInfo>
#include <QHash>
#include <QJsonArray>
#include <algorithm>
#include <cmath>

namespace beatfreaks {

namespace {

bool sameSong(const PlayedTrack& a, const PlayedTrack& b) {
    if (!a.filePath.isEmpty() || !b.filePath.isEmpty()) {
        return a.filePath == b.filePath;
    }
    return a.artist.compare(b.artist, Qt::CaseInsensitive) == 0 &&
            a.title.compare(b.title, Qt::CaseInsensitive) == 0;
}

QString csvField(QString s) {
    // Spreadsheet formula injection: a cell starting with = + - @ (or a tab /
    // carriage return before one) can run as a formula. Make it plain text.
    static const QString risky = QStringLiteral("=+-@\t\r");
    if (!s.isEmpty() && risky.contains(s.at(0))) {
        s.prepend(QLatin1Char('\''));
    }
    if (s.contains(QLatin1Char(',')) || s.contains(QLatin1Char('"')) ||
            s.contains(QLatin1Char('\n'))) {
        QString q = s;
        q.replace(QLatin1Char('"'), QStringLiteral("\"\""));
        return QLatin1Char('"') + q + QLatin1Char('"');
    }
    return s;
}

QString cueText(QString s) {
    s.replace(QLatin1Char('"'), QLatin1Char('\''));
    s.replace(QLatin1Char('\n'), QLatin1Char(' '));
    return s;
}

} // namespace

Tracklist::Tracklist(QList<PlayedTrack> played, TracklistOptions options)
        : m_options(std::move(options)) {
    // clean times first: a NaN would break the sort order
    for (PlayedTrack& t : played) {
        if (!std::isfinite(t.startSeconds) || t.startSeconds < 0) {
            t.startSeconds = 0;
        }
        if (!std::isfinite(t.playedSeconds) || t.playedSeconds < 0) {
            t.playedSeconds = 0;
        }
    }
    std::stable_sort(played.begin(), played.end(), [](const PlayedTrack& a, const PlayedTrack& b) {
        return a.startSeconds < b.startSeconds;
    });
    for (int i = 0; i < played.size(); ++i) {
        const PlayedTrack& t = played[i];
        const bool last = i == played.size() - 1;
        // unknown length: use the gap to the next song
        double length = t.playedSeconds;
        if (length <= 0 && !last) {
            length = played[i + 1].startSeconds - t.startSeconds;
        }
        if (!last && length < m_options.minPlaySeconds) {
            continue; // a preview or a mistake
        }
        if (!m_tracks.isEmpty() && sameSong(m_tracks.last(), t)) {
            continue; // reloaded / restarted the same song
        }
        m_tracks.append(t);
    }
}

QString Tracklist::formatTime(double seconds, bool forceHours) {
    const qint64 s = std::max<qint64>(0, static_cast<qint64>(std::floor(seconds)));
    const qint64 h = s / 3600;
    const qint64 m = (s % 3600) / 60;
    const qint64 sec = s % 60;
    if (h > 0 || forceHours) {
        return QStringLiteral("%1:%2:%3")
                .arg(h)
                .arg(m, 2, 10, QLatin1Char('0'))
                .arg(sec, 2, 10, QLatin1Char('0'));
    }
    return QStringLiteral("%1:%2").arg(m, 2, 10, QLatin1Char('0')).arg(sec, 2, 10, QLatin1Char('0'));
}

QString Tracklist::displayName(const PlayedTrack& t) {
    QString title = t.title.trimmed();
    if (title.isEmpty()) {
        title = QFileInfo(t.filePath).completeBaseName();
    }
    if (title.isEmpty()) {
        title = QStringLiteral("Unknown");
    }
    const QString artist = t.artist.trimmed();
    return artist.isEmpty() ? title : artist + QStringLiteral(" – ") + title;
}

TracklistStats Tracklist::stats(double mixLengthSeconds) const {
    TracklistStats st;
    st.tracks = static_cast<int>(m_tracks.size());
    for (const PlayedTrack& t : m_tracks) {
        if (t.bpm > 0) {
            st.minBpm = st.minBpm == 0 ? t.bpm : std::min(st.minBpm, t.bpm);
            st.maxBpm = std::max(st.maxBpm, t.bpm);
        }
        if (!t.key.isEmpty() && (st.keyJourney.isEmpty() || st.keyJourney.last() != t.key)) {
            st.keyJourney << t.key;
        }
        st.aiMade += t.aiMade ? 1 : 0;
    }
    if (mixLengthSeconds > 0) {
        st.lengthSeconds = mixLengthSeconds;
    } else if (!m_tracks.isEmpty()) {
        const PlayedTrack& last = m_tracks.last();
        st.lengthSeconds = last.startSeconds + std::max(0.0, last.playedSeconds);
    }
    return st;
}

QString Tracklist::toText() const {
    const bool hours = !m_tracks.isEmpty() && m_tracks.last().startSeconds >= 3600;
    QStringList lines;
    if (!m_options.mixTitle.isEmpty()) {
        lines << m_options.mixTitle;
    }
    QStringList by;
    if (!m_options.djName.isEmpty()) {
        by << m_options.djName;
    }
    if (!m_options.date.isEmpty()) {
        by << m_options.date;
    }
    if (!by.isEmpty()) {
        lines << by.join(QStringLiteral(" · "));
    }
    if (!lines.isEmpty()) {
        lines << QString();
    }
    for (int i = 0; i < m_tracks.size(); ++i) {
        const PlayedTrack& t = m_tracks[i];
        QString line = QStringLiteral("%1. [%2] %3")
                               .arg(i + 1, 2, 10, QLatin1Char('0'))
                               .arg(formatTime(t.startSeconds, hours), displayName(t));
        QStringList extra;
        if (!t.key.isEmpty()) {
            extra << t.key;
        }
        if (t.bpm > 0) {
            extra << QStringLiteral("%1 BPM").arg(std::lround(t.bpm));
        }
        if (t.aiMade) {
            extra << QStringLiteral("AI-made");
        }
        if (!extra.isEmpty()) {
            line += QStringLiteral(" (") + extra.join(QStringLiteral(", ")) + QLatin1Char(')');
        }
        lines << line;
    }
    if (m_options.creditBeatFreaks && !m_tracks.isEmpty()) {
        lines << QString() << QStringLiteral("Mixed live in BeatFreaks");
    }
    return lines.join(QLatin1Char('\n'));
}

QString Tracklist::toYouTubeChapters() const {
    // YouTube: first chapter at 0:00, at least 3 chapters, each ≥ 10 s.
    QList<PlayedTrack> chapters;
    for (const PlayedTrack& t : m_tracks) {
        if (chapters.isEmpty()) {
            PlayedTrack first = t;
            first.startSeconds = 0;
            chapters << first;
        } else if (t.startSeconds - chapters.last().startSeconds >= 10.0) {
            chapters << t;
        }
    }
    if (chapters.size() < 3) {
        return {};
    }
    const bool hours = chapters.last().startSeconds >= 3600;
    QStringList lines;
    for (const PlayedTrack& t : chapters) {
        lines << formatTime(t.startSeconds, hours) + QLatin1Char(' ') + displayName(t);
    }
    return lines.join(QLatin1Char('\n'));
}

QString Tracklist::toCue(const QString& audioFileName) const {
    QStringList lines;
    if (!m_options.djName.isEmpty()) {
        lines << QStringLiteral("PERFORMER \"%1\"").arg(cueText(m_options.djName));
    }
    lines << QStringLiteral("TITLE \"%1\"").arg(cueText(
            m_options.mixTitle.isEmpty() ? QStringLiteral("BeatFreaks mix") : m_options.mixTitle));
    QString type = QStringLiteral("WAVE");
    if (audioFileName.endsWith(QStringLiteral(".mp3"), Qt::CaseInsensitive)) {
        type = QStringLiteral("MP3");
    }
    lines << QStringLiteral("FILE \"%1\" %2").arg(cueText(QFileInfo(audioFileName).fileName()), type);
    for (int i = 0; i < m_tracks.size(); ++i) {
        const PlayedTrack& t = m_tracks[i];
        // CUE time is mm:ss:ff with 75 frames per second; minutes may pass 99
        const double start = i == 0 ? 0.0 : t.startSeconds;
        const qint64 frames = static_cast<qint64>(std::llround(start * 75.0));
        const qint64 mm = frames / (75 * 60);
        const qint64 ss = (frames / 75) % 60;
        const qint64 ff = frames % 75;
        lines << QStringLiteral("  TRACK %1 AUDIO").arg(i + 1, 2, 10, QLatin1Char('0'));
        lines << QStringLiteral("    TITLE \"%1\"").arg(cueText(t.title.isEmpty() ? displayName(t) : t.title));
        if (!t.artist.isEmpty()) {
            lines << QStringLiteral("    PERFORMER \"%1\"").arg(cueText(t.artist));
        }
        lines << QStringLiteral("    INDEX 01 %1:%2:%3")
                         .arg(mm, 2, 10, QLatin1Char('0'))
                         .arg(ss, 2, 10, QLatin1Char('0'))
                         .arg(ff, 2, 10, QLatin1Char('0'));
    }
    return lines.join(QLatin1Char('\n')) + QLatin1Char('\n');
}

QString Tracklist::toCsv() const {
    QStringList lines{QStringLiteral("#,Start,Artist,Title,Key,BPM,AI-made")};
    for (int i = 0; i < m_tracks.size(); ++i) {
        const PlayedTrack& t = m_tracks[i];
        lines << QStringList{QString::number(i + 1), formatTime(t.startSeconds, true),
                         csvField(t.artist), csvField(t.title), csvField(t.key),
                         t.bpm > 0 ? QString::number(t.bpm, 'f', 1) : QString(),
                         t.aiMade ? QStringLiteral("yes") : QStringLiteral("no")}
                         .join(QLatin1Char(','));
    }
    return lines.join(QLatin1Char('\n')) + QLatin1Char('\n');
}

QJsonObject Tracklist::toJson(double mixLengthSeconds) const {
    QJsonArray tracks;
    for (const PlayedTrack& t : m_tracks) {
        tracks.append(QJsonObject{
                {QStringLiteral("artist"), t.artist},
                {QStringLiteral("title"), t.title},
                {QStringLiteral("start"), t.startSeconds},
                {QStringLiteral("bpm"), t.bpm},
                {QStringLiteral("key"), t.key},
                {QStringLiteral("ai_made"), t.aiMade},
        });
    }
    const TracklistStats st = stats(mixLengthSeconds);
    return QJsonObject{
            {QStringLiteral("title"), m_options.mixTitle},
            {QStringLiteral("dj"), m_options.djName},
            {QStringLiteral("date"), m_options.date},
            {QStringLiteral("length"), st.lengthSeconds},
            {QStringLiteral("bpm_min"), st.minBpm},
            {QStringLiteral("bpm_max"), st.maxBpm},
            {QStringLiteral("key_journey"), QJsonArray::fromStringList(st.keyJourney)},
            {QStringLiteral("ai_made"), st.aiMade},
            {QStringLiteral("tracks"), tracks},
    };
}

MixCoverPlan MixCoverPlan::make(const Tracklist& list, const TracklistOptions& options, int size) {
    MixCoverPlan plan;
    plan.size = std::max(300, size);
    plan.title = options.mixTitle.isEmpty() ? QStringLiteral("BeatFreaks Mix") : options.mixTitle;
    QStringList sub;
    if (!options.djName.isEmpty()) {
        sub << options.djName;
    }
    if (!options.date.isEmpty()) {
        sub << options.date;
    }
    sub << QStringLiteral("%1 tracks").arg(list.tracks().size());
    plan.subtitle = sub.join(QStringLiteral(" · "));

    // distinct artworks, longest on air first (the songs that carried the set)
    QHash<QString, double> airTime;
    QStringList order;
    for (const PlayedTrack& t : list.tracks()) {
        if (!options.useTrackArtwork || t.coverArtPath.isEmpty()) {
            continue;
        }
        if (!airTime.contains(t.coverArtPath)) {
            order << t.coverArtPath;
        }
        airTime[t.coverArtPath] += std::max(1.0, t.playedSeconds);
    }
    std::stable_sort(order.begin(), order.end(), [&](const QString& a, const QString& b) {
        return airTime.value(a) > airTime.value(b);
    });
    const int n = static_cast<int>(order.size());
    plan.grid = n >= 9 ? 3 : n >= 4 ? 2 : n >= 1 ? 1 : 0;
    const int tile = plan.grid > 0 ? plan.size / plan.grid : 0;
    for (int i = 0; i < plan.grid * plan.grid; ++i) {
        plan.tiles << CoverTile{order[i], (i % plan.grid) * tile, (i / plan.grid) * tile, tile};
    }
    plan.titleBandHeight = plan.size / 5;
    plan.titleBandY = plan.size - plan.titleBandHeight;
    return plan;
}

} // namespace beatfreaks
