#pragma once

// DeckForge project file (.dfproj): clips on channels, markers and automation
// lanes, saved as readable JSON. Uses only QtCore.

#include <QJsonObject>
#include <QList>
#include <QString>
#include <optional>

#include "deckforge/automation/automationrecorder.h"

namespace deckforge {

/// One piece of audio on a channel of the timeline.
struct Clip {
    QString id;                     ///< stable id, e.g. "clip-3"
    int channel = 0;                ///< 0 = deck A ... 3 = deck D
    QString filePath;               ///< the audio file; stays where it is on disk
    QString label;                  ///< shown on the clip
    double startBeat = 0.0;         ///< where the clip starts on the timeline
    double lengthBeats = 0.0;       ///< how long it plays
    double sourceOffsetBeats = 0.0; ///< where in the song it starts
    double gainDb = 0.0;
    double pitchSemitones = 0.0;
    bool loop = false;

    double endBeat() const {
        return startBeat + lengthBeats;
    }
    bool operator==(const Clip& other) const = default;
};

/// A named spot on the timeline ("Bass swap").
struct Marker {
    double beat = 0.0;
    QString label;
    QString color; ///< "#RRGGBB"
    bool operator==(const Marker& other) const = default;
};

class DfProject {
  public:
    static constexpr int kFormatVersion = 1;
    static constexpr int kChannelCount = 4;

    QString title = QStringLiteral("Untitled mix");
    double bpm = 124.0;
    QList<Clip> clips;
    QList<Marker> markers;
    LaneMap lanes;

    /// The Mixxx group for a channel index: 0 -> "[Channel1]".
    static QString channelGroup(int channel);

    /// Move a clip and the automation recorded on its channel during the
    /// clip, so a dragged transition keeps its moves. Returns false if no
    /// clip has that id.
    bool moveClip(const QString& clipId, double deltaBeats);

    /// The last beat that holds a clip or an automation point.
    double lengthBeats() const;

    QJsonObject toJson() const;
    /// Returns std::nullopt and fills error if the JSON is not a DeckForge
    /// project or is from a newer version.
    static std::optional<DfProject> fromJson(const QJsonObject& json, QString* error = nullptr);

    /// Atomic save: writes to a temp file and swaps it in, so a crash
    /// mid-save never corrupts the project.
    bool save(const QString& path, QString* error = nullptr) const;
    static std::optional<DfProject> load(const QString& path, QString* error = nullptr);
};

} // namespace deckforge
