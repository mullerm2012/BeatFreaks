#pragma once

// Smart Next: "what should I play next?" — the owner's #1 feature.
// Plain C++17, no Qt: unit-tested in the fast core suite.
//
// Two parts:
//  - EnergyAnalyzer turns a track's audio into an energy score 0–10
//    (how hard it hits: loudness, low-end punch, beat density).
//  - SmartNext ranks candidate tracks against the one playing now by key
//    (Camelot wheel), BPM (incl. half/double time), the energy direction the
//    DJ wants, genre, and what moved the crowd before.

#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace beatfreaks {

// ---------------------------------------------------------------- Energy --

struct EnergyResult {
    double score = 0.0;        ///< 0 (ambient) … 10 (peak-time banger)
    double loudnessDb = -90.0; ///< average RMS level in dBFS
    double lowEndShare = 0.0;  ///< share of energy below ~150 Hz (0–1)
    double onsetsPerSecond = 0.0; ///< how busy the rhythm is
};

/// Streaming version: feed audio chunk by chunk (as Mixxx's analysis does)
/// without keeping the whole song in memory.
class EnergyAccumulator {
  public:
    explicit EnergyAccumulator(double sampleRate);
    /// Feed interleaved samples in [-1, 1]; channels are mixed to mono.
    void feed(const float* samples, std::size_t frameCount, int channels = 1);
    /// The result for everything fed so far. Needs at least one second.
    EnergyResult result() const;

  private:
    double m_sampleRate;
    double m_alpha;          ///< low-pass coefficient (~150 Hz)
    std::size_t m_frameLen;  ///< samples per 50 ms frame
    double m_low = 0.0;
    double m_totalSquares = 0.0;
    double m_lowSquares = 0.0;
    std::size_t m_samples = 0;
    double m_frameSum = 0.0;
    std::size_t m_inFrame = 0;
    // onset detection state
    std::size_t m_frames = 0;
    std::size_t m_onsets = 0;
    double m_average = 0.0;
    bool m_armed = true;
};

class EnergyAnalyzer {
  public:
    /// mono samples in [-1, 1]. Needs at least one second of audio.
    static EnergyResult analyze(const std::vector<float>& mono, double sampleRate);
};

// ------------------------------------------------------------- Camelot ----

/// A key on the Camelot wheel: number 1–12, letter A (minor) or B (major).
struct CamelotKey {
    int number = 0; ///< 1–12; 0 = unknown
    char letter = 'A';

    bool isValid() const {
        return number >= 1 && number <= 12 && (letter == 'A' || letter == 'B');
    }
    /// Parses "8A", "12b", " 3B ". Returns an invalid key otherwise.
    static CamelotKey parse(const std::string& text);
    std::string toString() const;
};

/// How well two keys mix: 1.0 same key; 0.9 one step around the wheel or
/// relative major/minor; 0.6 two steps / energy boost (+2); 0.0 clash.
/// Unknown keys score 0.5 (no opinion).
double keyCompatibility(const CamelotKey& from, const CamelotKey& to);

// ----------------------------------------------------------- SmartNext ----

struct TrackInfo {
    std::string id;     ///< library id or file path
    std::string title;
    std::string artist;
    std::string genre;
    double bpm = 0.0;   ///< 0 = unknown
    CamelotKey key;
    double energy = -1.0;     ///< 0–10, <0 = not analysed yet
    double crowdScore = 0.0;  ///< learned from "crowd hot/cold": -1 … +1
    int timesPlayedTonight = 0;
};

enum class EnergyDirection {
    Build,    ///< take it higher
    Hold,     ///< keep the vibe
    CoolDown, ///< bring it down
};

struct SmartNextSettings {
    EnergyDirection direction = EnergyDirection::Hold;
    /// Largest tempo change the DJ is happy to make, in percent.
    double maxBpmChangePercent = 6.0;
    bool allowHalfDoubleTime = true;
    bool sameGenreBonus = true;
    std::size_t maxResults = 10;
};

/// How a candidate's tempo lines up with the playing track.
struct BpmMatch {
    double changePercent = 0.0; ///< pitch change on the incoming track (signed)
    double multiplier = 1.0;    ///< 1 = same time, 2 = played double time, 0.5 = half time
};

/// One ranked candidate. Owns its data (no pointers into the library), so a
/// panel can keep it after the library snapshot is gone.
struct Suggestion {
    std::string trackId;
    std::string title;
    std::string artist;
    std::size_t candidateIndex = 0; ///< position in the list passed to rank()
    double score = 0.0;             ///< 0–100, higher is better
    BpmMatch bpm;                   ///< tempo match (only meaningful if both BPMs known)
    std::string reason;             ///< "Key 8A→9A · +1.6% · energy ↑"
};

class SmartNext {
  public:
    /// BPM match: the pitch change (percent) to bring `to` to `from`'s tempo,
    /// trying half/double time when allowed. nullopt if out of range or a
    /// BPM is unknown (<= 0 or not a number).
    static std::optional<BpmMatch> matchBpm(
            double fromBpm, double toBpm, const SmartNextSettings& settings);

    /// Rank candidates against the playing track. Skips the playing track
    /// itself, tracks with an empty id, and duplicate ids. Tracks already
    /// played tonight go last. Equal scores are ordered by title, then id,
    /// so the list never jumps around between refreshes.
    static std::vector<Suggestion> rank(const TrackInfo& playing,
            const std::vector<TrackInfo>& candidates,
            const SmartNextSettings& settings);
};

} // namespace beatfreaks
