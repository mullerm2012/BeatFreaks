#pragma once

// Model behind the Hardware Test screen: the list of controls we expect from
// each device, which ones have been heard, and a plain-text report.
// Plain C++17, no Qt, so the logic is unit-tested.

#include <map>
#include <string>
#include <vector>

namespace deckforge {

enum class CheckState {
    Waiting, ///< not heard yet
    Passed,  ///< heard / confirmed
    Failed,  ///< the user marked it as broken, or a self-check failed
};

struct HardwareControl {
    std::string device; ///< "S4 MK3" or "F1"
    std::string id;     ///< unique id, e.g. "S4.L.play"
    std::string label;  ///< shown on screen, e.g. "Play (left)"
    CheckState state = CheckState::Waiting;
    std::string note; ///< e.g. "LED did not light"
};

class HardwareTestModel {
  public:
    /// The controls DeckForge expects from a Traktor Kontrol S4 MK3 and an F1.
    static HardwareTestModel forS4Mk3AndF1();

    void add(const std::string& device, const std::string& id, const std::string& label);

    /// A control was heard. Returns false for an unknown id (worth logging:
    /// it usually means the mapping sends something we did not expect).
    bool markHeard(const std::string& id);
    bool markFailed(const std::string& id, const std::string& note);
    void reset();

    std::size_t total() const {
        return m_controls.size();
    }
    std::size_t count(CheckState state) const;
    std::size_t total(const std::string& device) const;
    std::size_t count(const std::string& device, CheckState state) const;
    bool allPassed() const {
        return count(CheckState::Passed) == total();
    }
    const std::vector<HardwareControl>& controls() const {
        return m_controls;
    }

    /// Plain-text checklist for the Send-log report: a summary line, then
    /// failed controls, then controls never heard.
    std::string report() const;

  private:
    std::vector<HardwareControl> m_controls;
    std::map<std::string, std::size_t> m_index;
};

} // namespace deckforge
