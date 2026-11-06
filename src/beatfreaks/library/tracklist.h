#pragma once

// Tracklist: "what did we play?" for every set and every recorded mix.
//
// BeatFreaks writes down each song as it goes live (Mixxx's History keeps
// the raw log). From that list this file makes everything a DJ posts after
// a set:
//   - a numbered tracklist with timestamps (SoundCloud / Mixcloud / socials)
//   - YouTube chapters (first one at 0:00, at least 10 s apart)
//   - a CUE sheet so the recorded mix splits into tracks in any player
//   - CSV for spreadsheets, JSON for the website / overlays
//   - set stats: length, BPM range, key journey, AI-made songs
// and a MixCoverPlan: which artwork goes where on the mix's cover.

#include <QJsonObject>
#include <QList>
#include <QString>
#include <QStringList>

namespace beatfreaks {

struct PlayedTrack {
    QString artist;
    QString title;
    QString filePath;         ///< for cover art and dedupe
    double startSeconds = 0;  ///< when it went live, from the start of the mix
    double playedSeconds = 0; ///< how long it was audible; 0 = unknown/still playing
    double bpm = 0;
    QString key;              ///< Camelot, e.g. "8A"
    QString coverArtPath;     ///< album art if the file has one
    bool aiMade = false;      ///< made by BeatFreaks AI / ACE-Step / Suno
};

struct TracklistOptions {
    /// Songs on air shorter than this are left out (a quick preview, a
    /// mistake). The last song is always kept.
    double minPlaySeconds = 30.0;
    QString mixTitle;
    QString djName;
    QString date;             ///< ISO date, e.g. "2026-11-06"
    bool creditBeatFreaks = true;
    /// Put the songs' album art on the mix cover. Off by default: owning a
    /// song does not mean you may publish its artwork, so the default cover
    /// is BeatFreaks' own art + title band. The UI shows a rights warning.
    bool useTrackArtwork = false;
};

struct TracklistStats {
    int tracks = 0;
    double lengthSeconds = 0;
    double minBpm = 0;
    double maxBpm = 0;
    QStringList keyJourney;   ///< Camelot keys in play order, repeats collapsed
    int aiMade = 0;
};

class Tracklist {
  public:
    Tracklist(QList<PlayedTrack> played, TracklistOptions options = {});

    /// The cleaned list: sorted by time, short plays and back-to-back
    /// repeats of the same song removed.
    const QList<PlayedTrack>& tracks() const {
        return m_tracks;
    }
    TracklistStats stats(double mixLengthSeconds = 0) const;

    /// "01. [00:00] Artist – Title (8A, 128 BPM)"
    QString toText() const;
    /// YouTube description chapters. Empty if fewer than 3 chapters fit.
    QString toYouTubeChapters() const;
    /// CUE sheet for `audioFileName` (the recorded mix).
    QString toCue(const QString& audioFileName) const;
    QString toCsv() const;
    QJsonObject toJson(double mixLengthSeconds = 0) const;

    /// 1:02:03 or 02:03
    static QString formatTime(double seconds, bool forceHours = false);
    static QString displayName(const PlayedTrack& t);

  private:
    QList<PlayedTrack> m_tracks;
    TracklistOptions m_options;
};

/// Where each artwork goes on a square mix cover.
struct CoverTile {
    QString artPath;
    int x = 0;
    int y = 0;
    int size = 0;
};

struct MixCoverPlan {
    int size = 3000;          ///< pixels, square (SoundCloud/Mixcloud like 3000)
    int grid = 0;             ///< 1, 2 or 3 tiles per side; 0 = no artwork
    QList<CoverTile> tiles;
    int titleBandY = 0;       ///< title/date band starts here
    int titleBandHeight = 0;
    QString title;
    QString subtitle;         ///< "DJ · date · N tracks"

    /// With options.useTrackArtwork: picks the most-played distinct artworks
    /// (up to 9) in the largest full square grid. Without it (default): no
    /// tiles, the renderer uses BeatFreaks' original art. The title band
    /// always covers the bottom fifth.
    static MixCoverPlan make(const Tracklist& list, const TracklistOptions& options, int size = 3000);
};

} // namespace beatfreaks
