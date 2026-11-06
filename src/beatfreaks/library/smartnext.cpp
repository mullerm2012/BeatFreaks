#include "beatfreaks/library/smartnext.h"

#include <algorithm>
#include <cctype>
#include <cmath>
#include <sstream>

namespace beatfreaks {

namespace {

double clamp01(double v) {
    return std::max(0.0, std::min(1.0, v));
}

} // namespace

// ---------------------------------------------------------------- Energy --

EnergyAccumulator::EnergyAccumulator(double sampleRate)
        : m_sampleRate(sampleRate > 0.0 ? sampleRate : 44100.0),
          // One-pole low-pass at ~150 Hz isolates the kick and bass.
          m_alpha(1.0 - std::exp(-2.0 * M_PI * 150.0 / m_sampleRate)),
          // 50 ms frames for loudness and onset detection.
          m_frameLen(std::max<std::size_t>(1, static_cast<std::size_t>(m_sampleRate * 0.05))) {
}

void EnergyAccumulator::feed(const float* samples, std::size_t frameCount, int channels) {
    if (samples == nullptr || channels < 1) {
        return;
    }
    for (std::size_t f = 0; f < frameCount; ++f) {
        double x = 0.0;
        for (int c = 0; c < channels; ++c) {
            x += samples[f * static_cast<std::size_t>(channels) + static_cast<std::size_t>(c)];
        }
        x /= channels;
        m_low += m_alpha * (x - m_low);
        m_totalSquares += x * x;
        m_lowSquares += m_low * m_low;
        m_frameSum += x * x;
        ++m_samples;
        if (++m_inFrame == m_frameLen) {
            const double e = m_frameSum / static_cast<double>(m_frameLen);
            // Onsets: frames whose energy jumps well above the recent average.
            if (m_frames > 0 && m_armed && e > 2.0 * m_average && e > 1e-6) {
                ++m_onsets;
                m_armed = false; // one onset per attack
            } else if (e < 1.2 * m_average) {
                m_armed = true;
            }
            m_average = m_frames == 0 ? e : 0.8 * m_average + 0.2 * e;
            ++m_frames;
            m_frameSum = 0.0;
            m_inFrame = 0;
        }
    }
}

EnergyResult EnergyAccumulator::result() const {
    EnergyResult result;
    if (m_samples < static_cast<std::size_t>(m_sampleRate)) {
        return result;
    }
    const double rms = std::sqrt(m_totalSquares / static_cast<double>(m_samples));
    result.loudnessDb = rms > 1e-9 ? 20.0 * std::log10(rms) : -90.0;
    result.lowEndShare = m_totalSquares > 1e-12 ? clamp01(m_lowSquares / m_totalSquares) : 0.0;
    const double seconds = static_cast<double>(m_samples) / m_sampleRate;
    result.onsetsPerSecond = static_cast<double>(m_onsets) / seconds;

    // Combine. Each part maps to 0–1, weighted to a 0–10 score.
    // Loudness: -30 dBFS (quiet) … -8 dBFS (club master).
    const double loud = clamp01((result.loudnessDb + 30.0) / 22.0);
    // Low end: 5% … 45% of the energy below 150 Hz.
    const double punch = clamp01((result.lowEndShare - 0.05) / 0.40);
    // Rhythm: 0 … 4 onsets per second (e.g. 120 BPM four-on-the-floor = 2).
    const double busy = clamp01(result.onsetsPerSecond / 4.0);
    result.score = 10.0 * (0.45 * loud + 0.30 * punch + 0.25 * busy);
    return result;
}

EnergyResult EnergyAnalyzer::analyze(const std::vector<float>& mono, double sampleRate) {
    if (sampleRate <= 0.0) {
        return EnergyResult{};
    }
    EnergyAccumulator acc(sampleRate);
    acc.feed(mono.data(), mono.size(), 1);
    return acc.result();
}

// ------------------------------------------------------------- Camelot ----

CamelotKey CamelotKey::parse(const std::string& text) {
    CamelotKey key;
    std::string t;
    for (char c : text) {
        if (!std::isspace(static_cast<unsigned char>(c))) {
            t.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(c))));
        }
    }
    if (t.size() < 2 || t.size() > 3) {
        return key;
    }
    const char letter = t.back();
    const std::string digits = t.substr(0, t.size() - 1);
    if (!std::all_of(digits.begin(), digits.end(), [](char c) {
            return std::isdigit(static_cast<unsigned char>(c));
        })) {
        return key;
    }
    const int number = std::stoi(digits);
    if (number < 1 || number > 12 || (letter != 'A' && letter != 'B')) {
        return key;
    }
    key.number = number;
    key.letter = letter;
    return key;
}

std::string CamelotKey::toString() const {
    return isValid() ? std::to_string(number) + letter : std::string("?");
}

double keyCompatibility(const CamelotKey& from, const CamelotKey& to) {
    if (!from.isValid() || !to.isValid()) {
        return 0.5;
    }
    // Steps around the wheel, 0–6.
    int diff = std::abs(from.number - to.number);
    diff = std::min(diff, 12 - diff);
    const bool sameLetter = from.letter == to.letter;
    if (sameLetter && diff == 0) {
        return 1.0;
    }
    if ((sameLetter && diff == 1) || (!sameLetter && diff == 0)) {
        return 0.9; // neighbour, or relative major/minor
    }
    if (sameLetter && diff == 2) {
        return 0.6; // "energy boost" jump
    }
    if (!sameLetter && diff == 1) {
        return 0.5; // diagonal move: works with care
    }
    return 0.0;
}

// ----------------------------------------------------------- SmartNext ----

std::optional<BpmMatch> SmartNext::matchBpm(
        double fromBpm, double toBpm, const SmartNextSettings& settings) {
    if (!(fromBpm > 0.0) || !(toBpm > 0.0) || !std::isfinite(fromBpm) ||
            !std::isfinite(toBpm)) {
        return std::nullopt; // unknown, zero, negative or NaN
    }
    std::vector<double> multipliers{1.0};
    if (settings.allowHalfDoubleTime) {
        multipliers.push_back(2.0);
        multipliers.push_back(0.5);
    }
    std::optional<BpmMatch> best;
    for (double m : multipliers) {
        // Pitch change needed on the incoming track to match the playing one.
        const double change = (fromBpm / (toBpm * m) - 1.0) * 100.0;
        if (std::fabs(change) <= settings.maxBpmChangePercent + 1e-9 &&
                (!best || std::fabs(change) < std::fabs(best->changePercent))) {
            best = BpmMatch{change, m};
        }
    }
    return best;
}

std::vector<Suggestion> SmartNext::rank(const TrackInfo& playing,
        const std::vector<TrackInfo>& candidates,
        const SmartNextSettings& settings) {
    std::vector<Suggestion> fresh;
    std::vector<Suggestion> repeats;
    std::vector<std::string> seen;
    const auto known = [](double bpm) { return bpm > 0.0 && std::isfinite(bpm); };
    for (std::size_t index = 0; index < candidates.size(); ++index) {
        const TrackInfo& track = candidates[index];
        if (track.id.empty() || track.id == playing.id ||
                std::find(seen.begin(), seen.end(), track.id) != seen.end()) {
            continue;
        }
        seen.push_back(track.id);
        const auto change = matchBpm(playing.bpm, track.bpm, settings);
        if (!change && known(playing.bpm) && known(track.bpm)) {
            continue; // tempo too far apart to mix
        }

        // Each part 0–1.
        const double keyPart = keyCompatibility(playing.key, track.key);
        const double bpmPart = change
                ? 1.0 - std::fabs(change->changePercent) /
                        std::max(0.1, settings.maxBpmChangePercent)
                : 0.5;

        double energyPart = 0.5;
        double energyDelta = 0.0;
        if (playing.energy >= 0.0 && track.energy >= 0.0) {
            energyDelta = track.energy - playing.energy;
            switch (settings.direction) {
            case EnergyDirection::Build:
                // Best: 0.5–2 points higher. Lower is poor.
                energyPart = energyDelta < 0.0
                        ? clamp01(0.4 + energyDelta / 5.0)
                        : clamp01(1.0 - std::fabs(energyDelta - 1.25) / 4.0);
                break;
            case EnergyDirection::Hold:
                energyPart = clamp01(1.0 - std::fabs(energyDelta) / 3.0);
                break;
            case EnergyDirection::CoolDown:
                energyPart = energyDelta > 0.0
                        ? clamp01(0.4 - energyDelta / 5.0)
                        : clamp01(1.0 - std::fabs(energyDelta + 1.25) / 4.0);
                break;
            }
        }

        const double crowdPart = clamp01(0.5 + 0.5 * track.crowdScore);
        const double genrePart = settings.sameGenreBonus && !playing.genre.empty() &&
                        playing.genre == track.genre
                ? 1.0
                : 0.5;

        Suggestion s;
        s.trackId = track.id;
        s.title = track.title;
        s.artist = track.artist;
        s.candidateIndex = index;
        if (change) {
            s.bpm = *change;
        }
        s.score = 100.0 *
                (0.32 * keyPart + 0.25 * bpmPart + 0.23 * energyPart + 0.12 * crowdPart +
                        0.08 * genrePart);

        std::ostringstream reason;
        reason << "Key " << playing.key.toString() << "→" << track.key.toString();
        if (change) {
            reason.setf(std::ios::fixed);
            reason.precision(1);
            reason << " · " << (change->changePercent >= 0 ? "+" : "")
                   << change->changePercent << "%";
            if (change->multiplier == 2.0) {
                reason << " (double time)";
            } else if (change->multiplier == 0.5) {
                reason << " (half time)";
            }
        }
        if (playing.energy >= 0.0 && track.energy >= 0.0) {
            reason << " · energy "
                   << (energyDelta > 0.25 ? "↑" : energyDelta < -0.25 ? "↓" : "=");
        }
        if (track.crowdScore > 0.3) {
            reason << " · crowd mover";
        }
        s.reason = reason.str();

        (track.timesPlayedTonight > 0 ? repeats : fresh).push_back(s);
    }
    auto byScore = [](const Suggestion& a, const Suggestion& b) {
        if (a.score != b.score) {
            return a.score > b.score;
        }
        if (a.title != b.title) {
            return a.title < b.title;
        }
        return a.trackId < b.trackId;
    };
    std::sort(fresh.begin(), fresh.end(), byScore);
    std::sort(repeats.begin(), repeats.end(), byScore);
    // Already-played tracks only fill up the list when nothing else fits.
    for (auto& r : repeats) {
        fresh.push_back(r);
    }
    if (fresh.size() > settings.maxResults) {
        fresh.resize(settings.maxResults);
    }
    return fresh;
}

} // namespace beatfreaks
