#pragma once

// "Send log to Claude": bundles everything needed to diagnose a problem into
// one plain-text file the user can attach in chat. Uses only QtCore.

#include <QDateTime>
#include <QString>
#include <QStringList>

namespace deckforge {

class DiagnosticReport {
  public:
    struct Input {
        QString appVersion;
        QString settingsDir;        ///< folder that holds mixxx.log, mixxx.log.1, ...
        QStringList controllers;    ///< connected controllers and their mapping
        QStringList audioDevices;   ///< sound API, device, buffer, sample rate
        QString hardwareTestReport; ///< HardwareTestModel::report()
        QString projectSummary;     ///< title, clips, lanes of the open project
        int logTailLines = 4000;    ///< lines kept from the newest log
        int previousLogTailLines = 1000; ///< lines kept from the previous run's log
        QString homePath;           ///< replaced by "~"; empty = QDir::homePath()
    };

    /// The full report text. Paths under the user's home folder are shortened
    /// to "~" so no personal folder names are shared.
    static QString buildText(const Input& input, const QDateTime& now);

    /// Writes DeckForge-report-<date>-<time>.txt into outDir and returns its
    /// path, or an empty string and an error.
    static QString write(const Input& input,
            const QString& outDir,
            const QDateTime& now,
            QString* error = nullptr);

    /// The last maxLines lines of a text file ("" if it cannot be read).
    static QString tail(const QString& path, int maxLines);
};

} // namespace deckforge
