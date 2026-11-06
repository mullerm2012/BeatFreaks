#include "beatfreaks/diagnostics/diagnosticreport.h"

#include <QDir>
#include <QFile>
#include <QSaveFile>
#include <QSysInfo>
#include <QTextStream>

namespace beatfreaks {

namespace {

void section(QString& out, const QString& title) {
    out += QStringLiteral("\n===== %1 =====\n").arg(title);
}

void list(QString& out, const QStringList& items) {
    if (items.isEmpty()) {
        out += QStringLiteral("(none found)\n");
        return;
    }
    for (const auto& item : items) {
        out += QStringLiteral("- %1\n").arg(item);
    }
}

} // namespace

QString DiagnosticReport::tail(const QString& path, int maxLines) {
    QFile file(path);
    if (maxLines <= 0 || !file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return {};
    }
    // Logs can be large; only keep the newest lines in memory.
    QStringList lines;
    QTextStream stream(&file);
    while (!stream.atEnd()) {
        lines.append(stream.readLine());
        if (lines.size() > maxLines) {
            lines.removeFirst();
        }
    }
    return lines.join('\n');
}

QString DiagnosticReport::buildText(const Input& input, const QDateTime& now) {
    QString out;
    out += QStringLiteral("BeatFreaks diagnostic report\n");
    out += QStringLiteral("Created: %1\n").arg(now.toString(Qt::ISODate));
    out += QStringLiteral("Version: %1\n").arg(input.appVersion);
    out += QStringLiteral("System: %1 (%2), CPU %3\n")
                   .arg(QSysInfo::prettyProductName(),
                           QSysInfo::kernelVersion(),
                           QSysInfo::currentCpuArchitecture());

    section(out, QStringLiteral("Controllers"));
    list(out, input.controllers);
    section(out, QStringLiteral("Audio"));
    list(out, input.audioDevices);
    section(out, QStringLiteral("Hardware Test"));
    out += input.hardwareTestReport.isEmpty() ? QStringLiteral("(not run)\n")
                                              : input.hardwareTestReport + '\n';
    section(out, QStringLiteral("Open project"));
    out += input.projectSummary.isEmpty() ? QStringLiteral("(none)\n")
                                          : input.projectSummary + '\n';

    const QDir dir(input.settingsDir);
    section(out, QStringLiteral("Log: this run (mixxx.log, newest lines)"));
    const QString current = tail(dir.filePath(QStringLiteral("mixxx.log")), input.logTailLines);
    out += current.isEmpty() ? QStringLiteral("(log not found)\n") : current + '\n';
    section(out, QStringLiteral("Log: previous run (mixxx.log.1)"));
    const QString previous = tail(dir.filePath(QStringLiteral("mixxx.log.1")),
            input.previousLogTailLines);
    out += previous.isEmpty() ? QStringLiteral("(log not found)\n") : previous + '\n';
    out += QStringLiteral("\n===== End of report =====\n");

    // Privacy: shorten the home folder in every path.
    const QString home = input.homePath.isEmpty() ? QDir::homePath() : input.homePath;
    if (!home.isEmpty() && home != QStringLiteral("/")) {
        out.replace(home, QStringLiteral("~"));
        out.replace(QDir::toNativeSeparators(home), QStringLiteral("~"));
    }
    return out;
}

QString DiagnosticReport::write(
        const Input& input, const QString& outDir, const QDateTime& now, QString* error) {
    QDir dir(outDir);
    if (!dir.exists() && !dir.mkpath(QStringLiteral("."))) {
        if (error != nullptr) {
            *error = QStringLiteral("Could not create %1").arg(outDir);
        }
        return {};
    }
    const QString path = dir.filePath(QStringLiteral("BeatFreaks-report-%1.txt")
                    .arg(now.toString(QStringLiteral("yyyyMMdd-HHmmss"))));
    QSaveFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        if (error != nullptr) {
            *error = file.errorString();
        }
        return {};
    }
    file.write(buildText(input, now).toUtf8());
    if (!file.commit()) {
        if (error != nullptr) {
            *error = file.errorString();
        }
        return {};
    }
    return path;
}

} // namespace beatfreaks
