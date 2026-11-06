#pragma once

// AI songs for the floor (board task A.12).
//
// A SongBrief describes the track BeatFreaks wants made: genre, mood, tempo,
// key, energy, vocals and a DJ-friendly structure (long intro and outro on
// phrase boundaries so Autopilot can mix it). The same brief becomes:
//   - a request for a local ACE-Step 1.5 server (MIT, free, runs offline), or
//   - a ready-to-paste Suno prompt ("Send to Suno").
//
// SongBrief::fromFloor() reads what is happening on the dance floor (tempo,
// key, energy, floor heat, what worked tonight) and writes the brief.

#include <QJsonObject>
#include <QString>
#include <QStringList>

#include "beatfreaks/library/smartnext.h"

namespace beatfreaks::ai {

/// What the song is for.
enum class SongPurpose {
    WarmUp,       ///< early night, deep and rolling
    FillTheFloor, ///< the floor went cold: catchy groove that pulls people in
    PeakBanger,   ///< take it higher: big drop, energy-boost key
    CoolDown,     ///< bring it down: melodic, relative key
    ToolTrack,    ///< drums + bass loop-friendly tool for long blends
};

enum class Vocals {
    None, ///< instrumental
    Hook, ///< a short vocal hook over an instrumental club track
    Full, ///< verses + chorus
};

/// What BeatFreaks knows about the floor right now.
struct FloorState {
    double bpm = 0.0;              ///< playing track tempo; 0 = unknown
    CamelotKey key;                ///< playing track key
    double energy = -1.0;          ///< playing track energy 0–10; <0 unknown
    double floorHeat = -1.0;       ///< crowd cam / crowd pads 0–1; <0 unknown
    QStringList genresThatWorked;  ///< most-liked first, e.g. {"tech house"}
};

struct SongBrief {
    QString genre = QStringLiteral("house");
    QString mood;              ///< e.g. "euphoric, big drop"
    QString extraStyle;        ///< free text from the DJ, appended to the style
    double bpm = 124.0;        ///< 30–300
    CamelotKey key;            ///< invalid = let the model pick
    double energy = 6.0;       ///< 0–10
    Vocals vocals = Vocals::Hook;
    QString theme;             ///< what the hook/lyrics are about
    QString lyrics;            ///< DJ's own lyrics; empty = model writes them
    QString language = QStringLiteral("en");
    bool djFriendly = true;    ///< long beat-only intro/outro for mixing
    int introBars = 32;
    int outroBars = 32;
    int totalBars = 128;       ///< whole song length in 4/4 bars
    int variations = 2;        ///< how many versions to make (1–8)
    bool cleanLyrics = true;   ///< no explicit words (club / radio safe)

    /// Builds a brief from the floor. Purpose decides energy/key/mood moves.
    static SongBrief fromFloor(const FloorState& floor, SongPurpose purpose);

    /// Length in seconds at this tempo, clamped to ACE-Step's 10–600 s.
    double durationSeconds() const;
    /// "A minor", "C# major" (ACE-Step's key_scale format); empty if unknown.
    QString keyScale() const;
    /// The style/caption sentence used for both ACE-Step and Suno.
    QString styleText() const;
    /// Lyrics with section tags ([Intro], [Build-Up], [Drop] …). If the DJ
    /// gave no lyrics and wants vocals, only the tags + a theme note are
    /// returned and the model writes the words.
    QString structuredLyrics() const;

    /// JSON body for ACE-Step 1.5 `POST /release_task`.
    QJsonObject toAceStepRequest() const;
    /// Text to paste into Suno: style line, then lyrics.
    QString toSunoPrompt() const;
    /// Short label for files and the library, e.g. "tech house 126 8A".
    QString label() const;
};

/// "8A" → "A minor" … ; empty string for an invalid key.
QString camelotToKeyScale(const CamelotKey& key);
/// Seconds for a number of 4/4 bars at a tempo (0 if bpm <= 0).
double barsToSeconds(int bars, double bpm);
QString purposeName(SongPurpose purpose);

} // namespace beatfreaks::ai
