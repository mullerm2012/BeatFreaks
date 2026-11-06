#include "beatfreaks/ai/songbrief.h"

#include <QStringBuilder>
#include <algorithm>
#include <cmath>

namespace beatfreaks::ai {

namespace {

// Camelot number → (minor key, major key)
constexpr const char* kMinor[12] = {"G# minor", "D# minor", "A# minor", "F minor",
        "C minor", "G minor", "D minor", "A minor", "E minor", "B minor",
        "F# minor", "C# minor"};
constexpr const char* kMajor[12] = {"B major", "F# major", "C# major", "G# major",
        "D# major", "A# major", "F major", "C major", "G major", "D major",
        "A major", "E major"};

CamelotKey stepKey(CamelotKey key, int steps) {
    if (!key.isValid()) {
        return key;
    }
    key.number = ((key.number - 1 + steps) % 12 + 12) % 12 + 1;
    return key;
}

CamelotKey relativeKey(CamelotKey key) {
    if (key.isValid()) {
        key.letter = key.letter == 'A' ? 'B' : 'A';
    }
    return key;
}

double clampEnergy(double e) {
    return std::clamp(e, 0.0, 10.0);
}

QString energyWords(double energy) {
    if (energy >= 8.5) {
        return QStringLiteral("peak-time, massive");
    }
    if (energy >= 6.5) {
        return QStringLiteral("driving, high energy");
    }
    if (energy >= 4.5) {
        return QStringLiteral("groovy, mid energy");
    }
    return QStringLiteral("deep, laid-back");
}

int roundToPhrase(int bars) {
    // keep sections on 8-bar phrase boundaries
    return std::max(8, (bars + 4) / 8 * 8);
}

} // namespace

QString camelotToKeyScale(const CamelotKey& key) {
    if (!key.isValid()) {
        return {};
    }
    return QString::fromLatin1(
            key.letter == 'A' ? kMinor[key.number - 1] : kMajor[key.number - 1]);
}

double barsToSeconds(int bars, double bpm) {
    if (bpm <= 0.0 || bars <= 0) {
        return 0.0;
    }
    return bars * 4.0 * 60.0 / bpm;
}

QString purposeName(SongPurpose purpose) {
    switch (purpose) {
    case SongPurpose::WarmUp:
        return QStringLiteral("Warm-up");
    case SongPurpose::FillTheFloor:
        return QStringLiteral("Fill the floor");
    case SongPurpose::PeakBanger:
        return QStringLiteral("Peak banger");
    case SongPurpose::CoolDown:
        return QStringLiteral("Cool down");
    case SongPurpose::ToolTrack:
        return QStringLiteral("DJ tool");
    }
    return {};
}

SongBrief SongBrief::fromFloor(const FloorState& floor, SongPurpose purpose) {
    SongBrief b;
    if (!floor.genresThatWorked.isEmpty() && !floor.genresThatWorked.first().trimmed().isEmpty()) {
        b.genre = floor.genresThatWorked.first().trimmed();
    }
    if (floor.bpm > 0.0 && std::isfinite(floor.bpm)) {
        // stay mixable: same tempo, rounded to a whole BPM
        b.bpm = std::clamp(std::round(floor.bpm), 30.0, 300.0);
    }
    const double now = floor.energy >= 0.0 ? floor.energy : 6.0;
    // a cold floor needs a bigger push than a warm one
    const bool cold = floor.floorHeat >= 0.0 && floor.floorHeat < 0.35;

    b.key = floor.key;
    switch (purpose) {
    case SongPurpose::WarmUp:
        b.energy = std::min(now, 5.0);
        b.mood = QStringLiteral("deep, rolling, hypnotic groove");
        b.vocals = Vocals::None;
        break;
    case SongPurpose::FillTheFloor:
        b.energy = clampEnergy(now + (cold ? 2.0 : 1.0));
        b.mood = QStringLiteral("irresistible groove, catchy hook, feel-good");
        b.vocals = Vocals::Hook;
        b.theme = QStringLiteral("everybody get on the dance floor");
        break;
    case SongPurpose::PeakBanger:
        b.energy = clampEnergy(std::max(now + 2.0, 8.5));
        b.key = stepKey(floor.key, 1); // Camelot energy boost: +1
        b.mood = QStringLiteral("euphoric build-up, huge drop, hands in the air");
        b.vocals = Vocals::Hook;
        b.theme = QStringLiteral("this is our moment, louder");
        break;
    case SongPurpose::CoolDown:
        b.energy = clampEnergy(now - 2.0);
        b.key = relativeKey(floor.key); // mood change, still in key
        b.mood = QStringLiteral("melodic, warm, uplifting, chilled");
        b.vocals = Vocals::Hook;
        b.theme = QStringLiteral("late night glow, one more song");
        break;
    case SongPurpose::ToolTrack:
        b.energy = clampEnergy(now);
        b.mood = QStringLiteral("minimal percussive DJ tool, drums and bass, loop-friendly");
        b.vocals = Vocals::None;
        b.totalBars = 96;
        break;
    }
    return b;
}

double SongBrief::durationSeconds() const {
    const double secs = barsToSeconds(totalBars, bpm);
    return std::clamp(secs, 10.0, 600.0);
}

QString SongBrief::keyScale() const {
    return camelotToKeyScale(key);
}

QString SongBrief::styleText() const {
    QStringList parts;
    parts << genre.trimmed();
    parts << energyWords(energy);
    if (!mood.trimmed().isEmpty()) {
        parts << mood.trimmed();
    }
    parts << QStringLiteral("%1 BPM").arg(static_cast<int>(std::lround(bpm)));
    if (key.isValid()) {
        parts << keyScale();
    }
    parts << QStringLiteral("4/4");
    switch (vocals) {
    case Vocals::None:
        parts << QStringLiteral("instrumental");
        break;
    case Vocals::Hook:
        parts << QStringLiteral("short vocal hook");
        break;
    case Vocals::Full:
        parts << QStringLiteral("vocals");
        break;
    }
    if (vocals != Vocals::None && lyrics.trimmed().isEmpty()) {
        // the theme goes in the style, never in the lyrics (it would be sung)
        if (!theme.trimmed().isEmpty()) {
            parts << QStringLiteral("lyrics about ") + theme.trimmed();
        }
        if (cleanLyrics) {
            parts << QStringLiteral("clean lyrics");
        }
    }
    if (djFriendly) {
        parts << QStringLiteral("club mix, DJ-friendly beat-only intro and outro");
    }
    if (!extraStyle.trimmed().isEmpty()) {
        parts << extraStyle.trimmed();
    }
    parts.removeAll(QString());
    return parts.join(QStringLiteral(", "));
}

QString SongBrief::structuredLyrics() const {
    const int intro = djFriendly ? roundToPhrase(introBars) : 8;
    const int outro = djFriendly ? roundToPhrase(outroBars) : 8;
    QStringList out;
    out << QStringLiteral("[Intro - %1 bars, drums only]").arg(intro);

    if (vocals == Vocals::None) {
        out << QStringLiteral("[Instrumental]") << QString()
            << QStringLiteral("[Build-Up]") << QString()
            << QStringLiteral("[Drop]") << QString()
            << QStringLiteral("[Breakdown]") << QString()
            << QStringLiteral("[Build-Up]") << QString()
            << QStringLiteral("[Drop]") << QString();
    } else if (!lyrics.trimmed().isEmpty()) {
        // DJ's own words: keep their section tags if they wrote any
        const QString own = lyrics.trimmed();
        if (!own.contains(QLatin1Char('['))) {
            out << QStringLiteral("[Verse]");
        }
        out << own << QString();
        if (!own.contains(QStringLiteral("[Drop]"), Qt::CaseInsensitive)) {
            out << QStringLiteral("[Drop]") << QString();
        }
    } else {
        // Let the model write the words; give it the shape and the theme.
        if (vocals == Vocals::Full) {
            out << QStringLiteral("[Verse 1]") << QString()
                << QStringLiteral("[Build-Up]") << QString()
                << QStringLiteral("[Chorus]") << QString()
                << QStringLiteral("[Drop]") << QString()
                << QStringLiteral("[Verse 2]") << QString()
                << QStringLiteral("[Breakdown]") << QString()
                << QStringLiteral("[Chorus]") << QString()
                << QStringLiteral("[Drop]") << QString();
        } else {
            out << QStringLiteral("[Build-Up]") << QString()
                << QStringLiteral("[Chorus - vocal hook]") << QString()
                << QStringLiteral("[Drop]") << QString()
                << QStringLiteral("[Breakdown]") << QString()
                << QStringLiteral("[Chorus - vocal hook]") << QString()
                << QStringLiteral("[Drop]") << QString();
        }
    }
    out << QStringLiteral("[Outro - %1 bars, drums only]").arg(outro);
    return out.join(QLatin1Char('\n'));
}

QJsonObject SongBrief::toAceStepRequest() const {
    QJsonObject req;
    req[QStringLiteral("prompt")] = styleText();
    req[QStringLiteral("bpm")] = static_cast<int>(std::lround(std::clamp(bpm, 30.0, 300.0)));
    if (key.isValid()) {
        req[QStringLiteral("key_scale")] = keyScale();
    }
    req[QStringLiteral("time_signature")] = QStringLiteral("4");
    req[QStringLiteral("audio_duration")] = durationSeconds();
    req[QStringLiteral("vocal_language")] = language;
    req[QStringLiteral("batch_size")] = std::clamp(variations, 1, 8);
    req[QStringLiteral("audio_format")] = QStringLiteral("flac"); // lossless for the club
    req[QStringLiteral("thinking")] = true; // best quality per ACE-Step docs
    req[QStringLiteral("use_random_seed")] = true;

    req[QStringLiteral("lyrics")] = structuredLyrics();
    // No words from the DJ: ACE-Step's language model writes them over our
    // section tags. Format mode keeps our BPM, key and length fixed.
    req[QStringLiteral("use_format")] = vocals != Vocals::None && lyrics.trimmed().isEmpty();
    return req;
}

QString SongBrief::toSunoPrompt() const {
    QString style = styleText();
    // Suno's style box is short; keep the most important words first.
    if (style.size() > 200) {
        style = style.left(197).trimmed() + QStringLiteral("...");
    }
    return QStringLiteral("Style:\n") % style % QStringLiteral("\n\nLyrics:\n") %
            structuredLyrics();
}

QString SongBrief::label() const {
    QString l = genre.trimmed() % QLatin1Char(' ') %
            QString::number(static_cast<int>(std::lround(bpm)));
    if (key.isValid()) {
        l += QLatin1Char(' ') + QString::fromStdString(key.toString());
    }
    return l;
}

} // namespace beatfreaks::ai
