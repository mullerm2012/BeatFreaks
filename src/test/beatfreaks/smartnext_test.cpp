#include <gtest/gtest.h>

#include <cmath>
#include <random>

#include "beatfreaks/library/smartnext.h"

using namespace beatfreaks;

namespace {

constexpr double kRate = 44100.0;

// A four-on-the-floor kick (decaying 55 Hz thump) at `bpm`, plus optional
// hats, for `seconds` seconds.
std::vector<float> kickPattern(double bpm, double seconds, float gain, bool hats) {
    std::vector<float> out(static_cast<std::size_t>(kRate * seconds), 0.0f);
    const double beat = 60.0 / bpm;
    std::mt19937 rng(7);
    std::uniform_real_distribution<float> noise(-1.0f, 1.0f);
    for (std::size_t i = 0; i < out.size(); ++i) {
        const double t = static_cast<double>(i) / kRate;
        const double sinceBeat = std::fmod(t, beat);
        double v = std::exp(-sinceBeat * 18.0) * std::sin(2.0 * M_PI * 55.0 * sinceBeat);
        if (hats) {
            const double sinceHat = std::fmod(t + beat / 2.0, beat);
            v += 0.15 * std::exp(-sinceHat * 60.0) * noise(rng);
        }
        out[i] = static_cast<float>(gain * v);
    }
    return out;
}

std::vector<float> softPad(double seconds) {
    std::vector<float> out(static_cast<std::size_t>(kRate * seconds));
    for (std::size_t i = 0; i < out.size(); ++i) {
        const double t = static_cast<double>(i) / kRate;
        out[i] = static_cast<float>(0.03 * std::sin(2.0 * M_PI * 440.0 * t));
    }
    return out;
}

TrackInfo track(const std::string& id, double bpm, const std::string& key, double energy,
        const std::string& genre = "moombahton") {
    TrackInfo t;
    t.id = id;
    t.title = id;
    t.bpm = bpm;
    t.key = CamelotKey::parse(key);
    t.energy = energy;
    t.genre = genre;
    return t;
}

} // namespace

TEST(EnergyAnalyzerTest, BangerScoresHigherThanAmbientPad) {
    const auto banger = EnergyAnalyzer::analyze(kickPattern(128, 8, 0.9f, true), kRate);
    const auto pad = EnergyAnalyzer::analyze(softPad(8), kRate);
    EXPECT_GT(banger.score, 6.0);
    EXPECT_LT(pad.score, 2.0);
    EXPECT_GT(banger.lowEndShare, 0.5);
    EXPECT_NEAR(128.0 / 60.0, banger.onsetsPerSecond, 0.6);
    EXPECT_LT(pad.onsetsPerSecond, 0.3);
}

TEST(EnergyAnalyzerTest, QuieterMixScoresLower) {
    const auto loud = EnergyAnalyzer::analyze(kickPattern(124, 8, 0.9f, false), kRate);
    const auto quiet = EnergyAnalyzer::analyze(kickPattern(124, 8, 0.1f, false), kRate);
    EXPECT_GT(loud.score, quiet.score + 2.0);
}

TEST(EnergyAnalyzerTest, TooShortOrSilentIsZero) {
    EXPECT_DOUBLE_EQ(0.0, EnergyAnalyzer::analyze(std::vector<float>(100, 0.5f), kRate).score);
    const auto silent = EnergyAnalyzer::analyze(std::vector<float>(44100 * 2, 0.0f), kRate);
    EXPECT_DOUBLE_EQ(0.0, silent.score);
}

TEST(CamelotTest, ParseAndPrint) {
    EXPECT_EQ("8A", CamelotKey::parse("8a").toString());
    EXPECT_EQ("12B", CamelotKey::parse(" 12B ").toString());
    EXPECT_FALSE(CamelotKey::parse("13A").isValid());
    EXPECT_FALSE(CamelotKey::parse("Am").isValid());
    EXPECT_FALSE(CamelotKey::parse("").isValid());
}

TEST(CamelotTest, CompatibilityFollowsTheWheel) {
    const auto k = [](const char* s) { return CamelotKey::parse(s); };
    EXPECT_DOUBLE_EQ(1.0, keyCompatibility(k("8A"), k("8A")));
    EXPECT_DOUBLE_EQ(0.9, keyCompatibility(k("8A"), k("9A")));
    EXPECT_DOUBLE_EQ(0.9, keyCompatibility(k("8A"), k("7A")));
    EXPECT_DOUBLE_EQ(0.9, keyCompatibility(k("8A"), k("8B")));
    EXPECT_DOUBLE_EQ(0.9, keyCompatibility(k("12A"), k("1A"))); // wraps around
    EXPECT_DOUBLE_EQ(0.6, keyCompatibility(k("8A"), k("10A")));
    EXPECT_DOUBLE_EQ(0.0, keyCompatibility(k("8A"), k("2B")));
    EXPECT_DOUBLE_EQ(0.5, keyCompatibility(k("8A"), CamelotKey{}));
}

TEST(SmartNextTest, BpmMatchIncludingHalfAndDoubleTime) {
    SmartNextSettings s;
    s.maxBpmChangePercent = 6.0;
    EXPECT_NEAR(0.0, *SmartNext::bpmChangePercent(124, 124, s), 1e-9);
    EXPECT_NEAR(124.0 / 122.0 * 100.0 - 100.0, *SmartNext::bpmChangePercent(124, 122, s), 1e-9);
    EXPECT_NEAR(0.0, *SmartNext::bpmChangePercent(124, 62, s), 1e-9); // half time
    EXPECT_FALSE(SmartNext::bpmChangePercent(124, 100, s).has_value());
    s.allowHalfDoubleTime = false;
    EXPECT_FALSE(SmartNext::bpmChangePercent(124, 62, s).has_value());
}

TEST(SmartNextTest, RanksKeyAndTempoMatchesFirst) {
    const TrackInfo playing = track("now", 124, "8A", 6.0);
    std::vector<TrackInfo> lib{
            track("clash", 124, "3B", 6.0),
            track("perfect", 124, "8A", 6.0),
            track("neighbour", 123, "9A", 6.2),
            track("too-slow", 95, "8A", 6.0),
            track("now", 124, "8A", 6.0), // the playing track itself
    };
    const auto ranked = SmartNext::rank(playing, lib, SmartNextSettings{});
    ASSERT_EQ(3u, ranked.size()); // "now" and "too-slow" are excluded
    EXPECT_EQ("perfect", ranked[0].track->id);
    EXPECT_EQ("neighbour", ranked[1].track->id);
    EXPECT_EQ("clash", ranked[2].track->id);
    EXPECT_NE(std::string::npos, ranked[1].reason.find("8A→9A"));
}

TEST(SmartNextTest, EnergyDirectionChangesTheOrder) {
    const TrackInfo playing = track("now", 124, "8A", 6.0);
    std::vector<TrackInfo> lib{
            track("harder", 124, "8A", 7.5),
            track("softer", 124, "8A", 4.5),
    };
    SmartNextSettings s;
    s.direction = EnergyDirection::Build;
    EXPECT_EQ("harder", SmartNext::rank(playing, lib, s)[0].track->id);
    s.direction = EnergyDirection::CoolDown;
    EXPECT_EQ("softer", SmartNext::rank(playing, lib, s)[0].track->id);
}

TEST(SmartNextTest, CrowdMoversAndRepeats) {
    const TrackInfo playing = track("now", 124, "8A", 6.0);
    std::vector<TrackInfo> lib{
            track("ok", 124, "8A", 6.0),
            track("mover", 124, "8A", 6.0),
            track("played", 124, "8A", 6.0),
    };
    lib[1].crowdScore = 1.0;
    lib[2].crowdScore = 1.0;
    lib[2].timesPlayedTonight = 1;
    const auto ranked = SmartNext::rank(playing, lib, SmartNextSettings{});
    ASSERT_EQ(3u, ranked.size());
    EXPECT_EQ("mover", ranked[0].track->id);
    EXPECT_EQ("played", ranked[2].track->id); // repeats go last, even if loved
    EXPECT_NE(std::string::npos, ranked[0].reason.find("crowd mover"));
}

TEST(SmartNextTest, MaxResults) {
    const TrackInfo playing = track("now", 124, "8A", 6.0);
    std::vector<TrackInfo> lib;
    for (int i = 0; i < 30; ++i) {
        lib.push_back(track("t" + std::to_string(i), 124, "8A", 6.0));
    }
    SmartNextSettings s;
    s.maxResults = 5;
    EXPECT_EQ(5u, SmartNext::rank(playing, lib, s).size());
}

TEST(EnergyAnalyzerTest, StreamingStereoMatchesOneShot) {
    const auto mono = kickPattern(126, 6, 0.8f, true);
    const auto oneShot = EnergyAnalyzer::analyze(mono, kRate);
    // Same audio as interleaved stereo, fed in odd-sized chunks.
    std::vector<float> stereo(mono.size() * 2);
    for (std::size_t i = 0; i < mono.size(); ++i) {
        stereo[2 * i] = mono[i];
        stereo[2 * i + 1] = mono[i];
    }
    EnergyAccumulator acc(kRate);
    std::size_t pos = 0;
    const std::size_t chunk = 1237;
    while (pos < mono.size()) {
        const std::size_t n = std::min(chunk, mono.size() - pos);
        acc.feed(stereo.data() + 2 * pos, n, 2);
        pos += n;
    }
    const auto streamed = acc.result();
    EXPECT_NEAR(oneShot.score, streamed.score, 1e-9);
    EXPECT_NEAR(oneShot.onsetsPerSecond, streamed.onsetsPerSecond, 1e-9);
}
