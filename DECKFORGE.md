# DeckForge

DeckForge is a fork of [Mixxx](https://github.com/mixxxdj/mixxx) that adds an
ACID-style **Arrange** timeline to a Traktor-style DJ program: every move you
make on your controller while DJing is recorded as editable automation, so you
can fix a mix after you play it, then export it.

DeckForge is an independent personal project. It is not affiliated with or
endorsed by the Mixxx project, Native Instruments or MAGIX. Like Mixxx, it is
licensed under the GNU GPL v2 or later (see `LICENSE` and `COPYING`).
Changes made for DeckForge are kept in this fork and are not sent upstream as
pull requests (see `AGENTS.md`).

- Plan, feature list, roadmap and test checklist:
  https://claude.ai/artifact/ReVS2FPEbinKF4BVB4FAFt
- Screen designs (DJ, Arrange, Hybrid, Hardware Test):
  https://claude.ai/artifact/7bnZSkmtDpCRtUuVDyFrhJ

## What is in this fork so far (Stage 0, part 1)

| Part | Files | State |
| --- | --- | --- |
| Automation lanes: points in beats, interpolation, thinning, moving with clips | `src/deckforge/automation/automationlane.*` | Done, tested |
| Recorder (Touch, Latch, punch-in, armed channels) and player | `src/deckforge/automation/automationrecorder.*` | Done, tested |
| Project file `.dfproj` (JSON, atomic save, version check) | `src/deckforge/project/dfproject.*` | Done, tested |
| Hardware Test model for S4 MK3 + F1 | `src/deckforge/diagnostics/hardwaretest.*` | Done, tested |
| Send-log report (logs, devices, test results; home folder hidden) | `src/deckforge/diagnostics/diagnosticreport.*` | Done, tested |
| Tagged log categories (`deckforge.recorder`, `deckforge.hw.s4`, ...) | `src/deckforge/diagnostics/dflog.*` | Done |
| Fast CI for the above | `.github/workflows/deckforge-core.yml`, `tools/deckforge_core/` | Done |

## Next (Stage 0, part 2)

1. Mixxx bridge: connect every `[ChannelN]` and `[EffectRackN]` control to the
   recorder through `ControlProxy`, and drive the player from the engine's
   beat position.
2. Menu items: Help → Hardware Test, Help → Send log to Claude, Controller Debug.
3. Rename the app and settings folder to DeckForge (keeps Mixxx settings
   separate) and set the installer name and icon.
4. First Windows installer from GitHub Actions for hardware testing.

## Building and testing

The core can be built and tested on its own in about a minute:

```sh
cmake -S tools/deckforge_core -B build-core -G Ninja
cmake --build build-core
ctest --test-dir build-core --output-on-failure
```

It needs Qt 6 (Core) and GoogleTest. The core is also part of the normal Mixxx
build (`mixxx-lib`) and test runner (`mixxx-test`).

The full app builds exactly like Mixxx; see `CONTRIBUTING.md`. On GitHub, the
existing "Pull request or branch build" workflow produces the Windows
installer as the `windows-win64` artifact for every pushed branch other than
`main`.
