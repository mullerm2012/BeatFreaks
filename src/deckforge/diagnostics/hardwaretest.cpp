#include "deckforge/diagnostics/hardwaretest.h"

#include <sstream>

namespace deckforge {

HardwareTestModel HardwareTestModel::forS4Mk3AndF1() {
    HardwareTestModel m;
    const std::string s4 = "S4 MK3";
    const char* sides[] = {"L", "R"};
    const char* sideNames[] = {"left", "right"};
    for (int s = 0; s < 2; ++s) {
        const std::string p = std::string("S4.") + sides[s] + ".";
        const std::string n = std::string(" (") + sideNames[s] + ")";
        const std::pair<const char*, const char*> deckControls[] = {
                {"play", "Play"},
                {"cue", "Cue"},
                {"sync", "Sync"},
                {"shift", "Shift"},
                {"deckselect", "Deck select"},
                {"hotcues", "Hotcues mode"},
                {"stems", "Stems mode"},
                {"tempo", "Tempo fader"},
                {"jog.touch", "Platter touch"},
                {"jog.turn", "Platter turn"},
                {"jog.motor", "Platter motor (Turntable mode)"},
                {"tt", "TT button"},
                {"jogmode", "Jog button"},
                {"flux", "Flux"},
                {"reverse", "Reverse"},
                {"grid", "Grid"},
                {"loop.turn", "Loop encoder turn"},
                {"loop.push", "Loop encoder push"},
                {"move.turn", "Move encoder turn"},
                {"move.push", "Move encoder push"},
                {"screen", "Screen"},
        };
        for (const auto& [id, label] : deckControls) {
            m.add(s4, p + id, label + n);
        }
        for (int pad = 1; pad <= 8; ++pad) {
            m.add(s4, p + "pad" + std::to_string(pad), "Pad " + std::to_string(pad) + n);
        }
    }
    for (int ch = 1; ch <= 4; ++ch) {
        const std::string p = "S4.M.ch" + std::to_string(ch) + ".";
        const std::string n = " ch" + std::to_string(ch);
        const std::pair<const char*, const char*> mixControls[] = {
                {"gain", "Gain"},
                {"hi", "EQ high"},
                {"mid", "EQ mid"},
                {"low", "EQ low"},
                {"fx", "Mixer FX knob"},
                {"fader", "Channel fader"},
                {"cue", "Headphone cue"},
        };
        for (const auto& [id, label] : mixControls) {
            m.add(s4, p + id, label + n);
        }
    }
    m.add(s4, "S4.M.xfader", "Crossfader");
    m.add(s4, "S4.M.browse.turn", "Browse encoder turn");
    m.add(s4, "S4.M.browse.push", "Browse encoder push");
    m.add(s4, "S4.audio.main", "Audio out: main");
    m.add(s4, "S4.audio.phones", "Audio out: headphones");

    const std::string f1 = "F1";
    for (int pad = 1; pad <= 16; ++pad) {
        m.add(f1, "F1.pad" + std::to_string(pad), "Pad " + std::to_string(pad));
    }
    for (int i = 1; i <= 4; ++i) {
        m.add(f1, "F1.knob" + std::to_string(i), "Knob " + std::to_string(i));
        m.add(f1, "F1.fader" + std::to_string(i), "Fader " + std::to_string(i));
        m.add(f1, "F1.stop" + std::to_string(i), "Stop " + std::to_string(i));
    }
    const std::pair<const char*, const char*> f1Buttons[] = {
            {"F1.browse.turn", "Browse encoder turn"},
            {"F1.browse.push", "Browse encoder push"},
            {"F1.shift", "Shift"},
            {"F1.sync", "Sync"},
            {"F1.quant", "Quant"},
            {"F1.capture", "Capture"},
            {"F1.type", "Type"},
            {"F1.reverse", "Reverse"},
            {"F1.size", "Size"},
            {"F1.browse", "Browse"},
    };
    for (const auto& [id, label] : f1Buttons) {
        m.add(f1, id, label);
    }
    return m;
}

void HardwareTestModel::add(
        const std::string& device, const std::string& id, const std::string& label) {
    if (m_index.count(id) > 0) {
        return;
    }
    m_index[id] = m_controls.size();
    m_controls.push_back(HardwareControl{device, id, label, CheckState::Waiting, {}});
}

bool HardwareTestModel::markHeard(const std::string& id) {
    const auto it = m_index.find(id);
    if (it == m_index.end()) {
        return false;
    }
    HardwareControl& control = m_controls[it->second];
    // A control the user marked as broken stays failed until reset, so a
    // flaky button does not hide behind one lucky press.
    if (control.state != CheckState::Failed) {
        control.state = CheckState::Passed;
    }
    return true;
}

bool HardwareTestModel::markFailed(const std::string& id, const std::string& note) {
    const auto it = m_index.find(id);
    if (it == m_index.end()) {
        return false;
    }
    m_controls[it->second].state = CheckState::Failed;
    m_controls[it->second].note = note;
    return true;
}

void HardwareTestModel::reset() {
    for (auto& c : m_controls) {
        c.state = CheckState::Waiting;
        c.note.clear();
    }
}

std::size_t HardwareTestModel::count(CheckState state) const {
    std::size_t n = 0;
    for (const auto& c : m_controls) {
        n += c.state == state ? 1 : 0;
    }
    return n;
}

std::size_t HardwareTestModel::total(const std::string& device) const {
    std::size_t n = 0;
    for (const auto& c : m_controls) {
        n += c.device == device ? 1 : 0;
    }
    return n;
}

std::size_t HardwareTestModel::count(const std::string& device, CheckState state) const {
    std::size_t n = 0;
    for (const auto& c : m_controls) {
        n += (c.device == device && c.state == state) ? 1 : 0;
    }
    return n;
}

std::string HardwareTestModel::report() const {
    std::ostringstream out;
    out << "Hardware Test: " << count(CheckState::Passed) << " of " << total()
        << " controls passed";
    for (const char* device : {"S4 MK3", "F1"}) {
        out << " | " << device << " " << count(device, CheckState::Passed) << "/"
            << total(device);
    }
    out << "\n";
    if (count(CheckState::Failed) > 0) {
        out << "FAILED:\n";
        for (const auto& c : m_controls) {
            if (c.state == CheckState::Failed) {
                out << "  [x] " << c.device << " | " << c.label << " (" << c.id << ")";
                if (!c.note.empty()) {
                    out << " - " << c.note;
                }
                out << "\n";
            }
        }
    }
    if (count(CheckState::Waiting) > 0) {
        out << "NEVER HEARD:\n";
        for (const auto& c : m_controls) {
            if (c.state == CheckState::Waiting) {
                out << "  [ ] " << c.device << " | " << c.label << " (" << c.id << ")\n";
            }
        }
    }
    return out.str();
}

} // namespace deckforge
