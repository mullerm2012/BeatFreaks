#pragma once

// Tagged log categories for BeatFreaks's own parts. Every line written through
// these lands in mixxx.log with its tag, e.g.
//   12:04:31.218 info [beatfreaks.recorder] lane=[Channel1],pitch beat=64.25 value=0.031
// so a Send-log report can be searched by part.

#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(bfArrange)  // timeline editing
Q_DECLARE_LOGGING_CATEGORY(bfRecorder) // automation recording
Q_DECLARE_LOGGING_CATEGORY(bfPlayer)   // automation playback
Q_DECLARE_LOGGING_CATEGORY(bfExport)   // rendering mixes and stems
Q_DECLARE_LOGGING_CATEGORY(bfHwS4)     // Traktor Kontrol S4 MK3
Q_DECLARE_LOGGING_CATEGORY(bfHwF1)     // Traktor Kontrol F1
Q_DECLARE_LOGGING_CATEGORY(bfTest)     // Hardware Test screen
Q_DECLARE_LOGGING_CATEGORY(bfLibrary)  // library analysis, Smart Next
Q_DECLARE_LOGGING_CATEGORY(bfAi)       // AI songs (ACE-Step), Song Inbox
