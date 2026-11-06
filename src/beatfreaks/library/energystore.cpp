#include "beatfreaks/library/energystore.h"

#include <QSqlError>
#include <QSqlQuery>

namespace beatfreaks {

EnergyStore::EnergyStore(const QSqlDatabase& db)
        : m_db(db) {
}

bool EnergyStore::ensureTable() {
    QSqlQuery query(m_db);
    return query.exec(QStringLiteral(
            "CREATE TABLE IF NOT EXISTS beatfreaks_track_energy ("
            " track_id INTEGER PRIMARY KEY,"
            " energy REAL NOT NULL,"
            " loudness_db REAL NOT NULL,"
            " low_end_share REAL NOT NULL,"
            " onsets_per_second REAL NOT NULL,"
            " analyzer_version INTEGER NOT NULL)"));
}

std::optional<EnergyResult> EnergyStore::load(const QVariant& trackId) const {
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
            "SELECT energy, loudness_db, low_end_share, onsets_per_second "
            "FROM beatfreaks_track_energy "
            "WHERE track_id = :id AND analyzer_version = :version"));
    query.bindValue(QStringLiteral(":id"), trackId);
    query.bindValue(QStringLiteral(":version"), kAnalyzerVersion);
    if (!query.exec() || !query.next()) {
        return std::nullopt;
    }
    EnergyResult result;
    result.score = query.value(0).toDouble();
    result.loudnessDb = query.value(1).toDouble();
    result.lowEndShare = query.value(2).toDouble();
    result.onsetsPerSecond = query.value(3).toDouble();
    return result;
}

bool EnergyStore::save(const QVariant& trackId, const EnergyResult& result) {
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral(
            "INSERT OR REPLACE INTO beatfreaks_track_energy "
            "(track_id, energy, loudness_db, low_end_share, onsets_per_second, "
            "analyzer_version) VALUES (:id, :energy, :loudness, :low, :onsets, :version)"));
    query.bindValue(QStringLiteral(":id"), trackId);
    query.bindValue(QStringLiteral(":energy"), result.score);
    query.bindValue(QStringLiteral(":loudness"), result.loudnessDb);
    query.bindValue(QStringLiteral(":low"), result.lowEndShare);
    query.bindValue(QStringLiteral(":onsets"), result.onsetsPerSecond);
    query.bindValue(QStringLiteral(":version"), kAnalyzerVersion);
    return query.exec();
}

bool EnergyStore::remove(const QVariant& trackId) {
    QSqlQuery query(m_db);
    query.prepare(QStringLiteral("DELETE FROM beatfreaks_track_energy WHERE track_id = :id"));
    query.bindValue(QStringLiteral(":id"), trackId);
    return query.exec();
}

} // namespace beatfreaks
