#include <gtest/gtest.h>

#include <QCoreApplication>
#include <QSqlDatabase>
#include <QSqlQuery>

#include "beatfreaks/library/energystore.h"

using namespace beatfreaks;

namespace {

QSqlDatabase memoryDb(const QString& name) {
    auto db = QSqlDatabase::addDatabase(QStringLiteral("QSQLITE"), name);
    db.setDatabaseName(QStringLiteral(":memory:"));
    return db;
}

} // namespace

TEST(EnergyStoreTest, SaveLoadAndVersioning) {
    {
        auto db = memoryDb(QStringLiteral("energy-test"));
        ASSERT_TRUE(db.open());
        EnergyStore store(db);
        ASSERT_TRUE(store.ensureTable());
        ASSERT_TRUE(store.ensureTable()); // idempotent

        EXPECT_FALSE(store.load(42).has_value());
        EnergyResult r;
        r.score = 7.25;
        r.loudnessDb = -9.5;
        r.lowEndShare = 0.4;
        r.onsetsPerSecond = 2.1;
        ASSERT_TRUE(store.save(42, r));
        const auto loaded = store.load(42);
        ASSERT_TRUE(loaded.has_value());
        EXPECT_DOUBLE_EQ(7.25, loaded->score);
        EXPECT_DOUBLE_EQ(-9.5, loaded->loudnessDb);

        r.score = 3.0; // re-analysis replaces the row
        ASSERT_TRUE(store.save(42, r));
        EXPECT_DOUBLE_EQ(3.0, store.load(42)->score);

        // Results from an older analyzer version are ignored.
        QSqlQuery q(db);
        ASSERT_TRUE(q.exec(QStringLiteral(
                "UPDATE beatfreaks_track_energy SET analyzer_version = 0")));
        EXPECT_FALSE(store.load(42).has_value());

        ASSERT_TRUE(store.remove(42));
        db.close();
    }
    QSqlDatabase::removeDatabase(QStringLiteral("energy-test"));
}
