#pragma once

#include <QSqlDatabase>
#include <memory>

#include "analyzer/analyzer.h"
#include "beatfreaks/library/energystore.h"
#include "beatfreaks/library/smartnext.h"

/// BeatFreaks: computes each track's energy score (0-10) during library
/// analysis, next to BPM and key, and stores it for Smart Next.
class AnalyzerEnergy : public Analyzer {
  public:
    explicit AnalyzerEnergy(const QSqlDatabase& dbConnection);
    ~AnalyzerEnergy() override = default;

    bool initialize(const AnalyzerTrack& track,
            mixxx::audio::SampleRate sampleRate,
            mixxx::audio::ChannelCount channelCount,
            SINT frameLength) override;
    bool processSamples(const CSAMPLE* pIn, SINT count) override;
    void storeResults(TrackPointer pTrack) override;
    void cleanup() override;

  private:
    beatfreaks::EnergyStore m_store;
    bool m_tableReady = false;
    int m_channels = 2;
    std::unique_ptr<beatfreaks::EnergyAccumulator> m_pAccumulator;
};
