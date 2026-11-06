#include "analyzer/analyzerenergy.h"

#include "analyzer/analyzertrack.h"
#include "beatfreaks/diagnostics/bflog.h"
#include "track/track.h"

AnalyzerEnergy::AnalyzerEnergy(const QSqlDatabase& dbConnection)
        : m_store(dbConnection) {
}

bool AnalyzerEnergy::initialize(const AnalyzerTrack& track,
        mixxx::audio::SampleRate sampleRate,
        mixxx::audio::ChannelCount channelCount,
        SINT frameLength) {
    Q_UNUSED(frameLength);
    if (!m_tableReady) {
        m_tableReady = m_store.ensureTable();
        if (!m_tableReady) {
            qCWarning(bfLibrary) << "Energy analysis disabled: could not create table";
            return false;
        }
    }
    const TrackPointer& pTrack = track.getTrack();
    if (!pTrack || !pTrack->getId().isValid()) {
        return false;
    }
    if (m_store.load(pTrack->getId().toVariant())) {
        return false; // already analysed with the current analyzer version
    }
    m_channels = static_cast<int>(channelCount);
    m_pAccumulator = std::make_unique<beatfreaks::EnergyAccumulator>(
            static_cast<double>(sampleRate));
    return true;
}

bool AnalyzerEnergy::processSamples(const CSAMPLE* pIn, SINT count) {
    if (!m_pAccumulator || m_channels < 1) {
        return false;
    }
    m_pAccumulator->feed(pIn, static_cast<std::size_t>(count / m_channels), m_channels);
    return true;
}

void AnalyzerEnergy::storeResults(TrackPointer pTrack) {
    if (!m_pAccumulator || !pTrack) {
        return;
    }
    const beatfreaks::EnergyResult result = m_pAccumulator->result();
    if (!m_store.save(pTrack->getId().toVariant(), result)) {
        qCWarning(bfLibrary) << "Could not save energy for track" << pTrack->getId();
        return;
    }
    qCInfo(bfLibrary) << "energy" << result.score << "loudnessDb" << result.loudnessDb
                      << "track" << pTrack->getId();
}

void AnalyzerEnergy::cleanup() {
    m_pAccumulator.reset();
}
