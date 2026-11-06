#include "beatfreaks/project/bfproject.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSaveFile>
#include <algorithm>

namespace beatfreaks {

namespace {

const QString kFormatName = QStringLiteral("beatfreaks-project");

QString shapeName(CurveShape shape) {
    return shape == CurveShape::Hold ? QStringLiteral("hold") : QStringLiteral("linear");
}

CurveShape shapeFromName(const QString& name) {
    return name == QLatin1String("hold") ? CurveShape::Hold : CurveShape::Linear;
}

void setError(QString* error, const QString& message) {
    if (error != nullptr) {
        *error = message;
    }
}

} // namespace

QString BfProject::channelGroup(int channel) {
    return QStringLiteral("[Channel%1]").arg(channel + 1);
}

bool BfProject::moveClip(const QString& clipId, double deltaBeats) {
    auto it = std::find_if(clips.begin(), clips.end(), [&](const Clip& c) {
        return c.id == clipId;
    });
    if (it == clips.end()) {
        return false;
    }
    // A clip cannot move before the start of the timeline; its automation
    // moves by exactly the same (possibly shortened) distance.
    const double delta = std::max(deltaBeats, -it->startBeat);
    const std::string group = channelGroup(it->channel).toStdString();
    for (auto& [key, lane] : lanes) {
        if (lane.group() == group) {
            lane.shiftRange(it->startBeat, it->endBeat(), delta);
        }
    }
    it->startBeat += delta;
    return true;
}

double BfProject::lengthBeats() const {
    double end = 0.0;
    for (const auto& clip : clips) {
        end = std::max(end, clip.endBeat());
    }
    for (const auto& [key, lane] : lanes) {
        if (!lane.isEmpty()) {
            end = std::max(end, lane.points().back().beat);
        }
    }
    return end;
}

QJsonObject BfProject::toJson() const {
    QJsonArray clipArray;
    for (const auto& clip : clips) {
        clipArray.append(QJsonObject{
                {"id", clip.id},
                {"channel", clip.channel},
                {"file", clip.filePath},
                {"label", clip.label},
                {"startBeat", clip.startBeat},
                {"lengthBeats", clip.lengthBeats},
                {"sourceOffsetBeats", clip.sourceOffsetBeats},
                {"gainDb", clip.gainDb},
                {"pitchSemitones", clip.pitchSemitones},
                {"loop", clip.loop},
        });
    }
    QJsonArray markerArray;
    for (const auto& marker : markers) {
        markerArray.append(QJsonObject{
                {"beat", marker.beat},
                {"label", marker.label},
                {"color", marker.color},
        });
    }
    QJsonArray laneArray;
    for (const auto& [key, lane] : lanes) {
        QJsonArray points;
        for (const auto& p : lane.points()) {
            // [beat, value] for ramps, [beat, value, "hold"] for steps: compact
            // enough for long sets, still readable.
            QJsonArray point{p.beat, p.value};
            if (p.shape == CurveShape::Hold) {
                point.append(shapeName(p.shape));
            }
            points.append(point);
        }
        laneArray.append(QJsonObject{
                {"control", QString::fromStdString(key)},
                {"toggle", lane.isToggle()},
                {"points", points},
        });
    }
    return QJsonObject{
            {"format", kFormatName},
            {"version", kFormatVersion},
            {"title", title},
            {"bpm", bpm},
            {"clips", clipArray},
            {"markers", markerArray},
            {"lanes", laneArray},
    };
}

std::optional<BfProject> BfProject::fromJson(const QJsonObject& json, QString* error) {
    if (json.value("format").toString() != kFormatName) {
        setError(error, QStringLiteral("Not a BeatFreaks project file."));
        return std::nullopt;
    }
    const int version = json.value("version").toInt(0);
    if (version < 1 || version > kFormatVersion) {
        setError(error,
                QStringLiteral("Project version %1 is not supported by this build "
                               "(supports up to %2). Update BeatFreaks.")
                        .arg(version)
                        .arg(kFormatVersion));
        return std::nullopt;
    }
    BfProject project;
    project.title = json.value("title").toString(project.title);
    project.bpm = json.value("bpm").toDouble(project.bpm);
    if (project.bpm <= 0.0) {
        setError(error, QStringLiteral("Project tempo must be above 0 BPM."));
        return std::nullopt;
    }
    for (const auto& value : json.value("clips").toArray()) {
        const QJsonObject o = value.toObject();
        Clip clip;
        clip.id = o.value("id").toString();
        clip.channel = o.value("channel").toInt();
        clip.filePath = o.value("file").toString();
        clip.label = o.value("label").toString();
        clip.startBeat = o.value("startBeat").toDouble();
        clip.lengthBeats = o.value("lengthBeats").toDouble();
        clip.sourceOffsetBeats = o.value("sourceOffsetBeats").toDouble();
        clip.gainDb = o.value("gainDb").toDouble();
        clip.pitchSemitones = o.value("pitchSemitones").toDouble();
        clip.loop = o.value("loop").toBool();
        if (clip.channel < 0 || clip.channel >= kChannelCount) {
            setError(error, QStringLiteral("Clip %1 is on channel %2; only 1-4 exist.")
                                    .arg(clip.id)
                                    .arg(clip.channel + 1));
            return std::nullopt;
        }
        project.clips.append(clip);
    }
    for (const auto& value : json.value("markers").toArray()) {
        const QJsonObject o = value.toObject();
        project.markers.append(Marker{o.value("beat").toDouble(),
                o.value("label").toString(),
                o.value("color").toString()});
    }
    for (const auto& value : json.value("lanes").toArray()) {
        const QJsonObject o = value.toObject();
        const std::string key = o.value("control").toString().toStdString();
        if (key.empty()) {
            continue;
        }
        AutomationLane lane(key, o.value("toggle").toBool());
        for (const auto& pv : o.value("points").toArray()) {
            const QJsonArray p = pv.toArray();
            if (p.size() < 2) {
                continue;
            }
            const CurveShape shape = p.size() > 2 ? shapeFromName(p.at(2).toString())
                                                  : CurveShape::Linear;
            lane.insert(AutomationPoint{p.at(0).toDouble(), p.at(1).toDouble(), shape});
        }
        project.lanes.insert_or_assign(key, std::move(lane));
    }
    return project;
}

bool BfProject::save(const QString& path, QString* error) const {
    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        setError(error, file.errorString());
        return false;
    }
    file.write(QJsonDocument(toJson()).toJson(QJsonDocument::Indented));
    if (!file.commit()) {
        setError(error, file.errorString());
        return false;
    }
    return true;
}

std::optional<BfProject> BfProject::load(const QString& path, QString* error) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        setError(error, file.errorString());
        return std::nullopt;
    }
    QJsonParseError parseError;
    const QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        setError(error,
                QStringLiteral("The project file is damaged (%1).")
                        .arg(parseError.errorString()));
        return std::nullopt;
    }
    return fromJson(doc.object(), error);
}

} // namespace beatfreaks
