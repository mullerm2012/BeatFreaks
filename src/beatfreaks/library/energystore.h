#pragma once

// Stores each track's BeatFreaks energy analysis in the library database
// (its own table, so Mixxx's schema is untouched). QtCore + QtSql only.

#include <QSqlDatabase>
#include <QVariant>
#include <optional>

#include "beatfreaks/library/smartnext.h"

namespace beatfreaks {

class EnergyStore {
  public:
    /// Bump when the analyzer changes so old results are recomputed.
    static constexpr int kAnalyzerVersion = 1;

    explicit EnergyStore(const QSqlDatabase& db);

    /// Creates the table if needed. Returns false on a database error.
    bool ensureTable();

    /// Result for a track if it was analysed with the current version.
    std::optional<EnergyResult> load(const QVariant& trackId) const;
    bool save(const QVariant& trackId, const EnergyResult& result);
    bool remove(const QVariant& trackId);

  private:
    QSqlDatabase m_db;
};

} // namespace beatfreaks
