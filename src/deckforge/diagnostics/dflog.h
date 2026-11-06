#pragma once

// Tagged log categories for DeckForge's own parts. Every line written through
// these lands in mixxx.log with its tag, e.g.
//   12:04:31.218 info [deckforge.recorder] lane=[Channel1],pitch beat=64.25 value=0.031
// so a Send-log report can be searched by part.

#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(dfArrange)  // timeline editing
Q_DECLARE_LOGGING_CATEGORY(dfRecorder) // automation recording
Q_DECLARE_LOGGING_CATEGORY(dfPlayer)   // automation playback
Q_DECLARE_LOGGING_CATEGORY(dfExport)   // rendering mixes and stems
Q_DECLARE_LOGGING_CATEGORY(dfHwS4)     // Traktor Kontrol S4 MK3
Q_DECLARE_LOGGING_CATEGORY(dfHwF1)     // Traktor Kontrol F1
Q_DECLARE_LOGGING_CATEGORY(dfTest)     // Hardware Test screen
