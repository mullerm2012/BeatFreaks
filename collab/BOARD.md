# BeatFreaks task board

States: **Open** · **In progress** · **In review** · **Blocked** · **Done**.
Claim a task by putting your name in **Owner** and pushing that change
before you start. Stages follow the
[product plan](https://claude.ai/artifact/ReVS2FPEbinKF4BVB4FAFt).

## Stage 0 — Foundation

| # | Task | Owner | State | Notes |
| --- | --- | --- | --- | --- |
| 0.1 | Automation core: lanes, recorder, player, `.bfproj`, Hardware Test model, Send-log report | Claude Code | Done | 36 tests, also under ASan/UBSan; `src/beatfreaks/` |
| 0.2 | Windows x64 installer from GitHub Actions on push to `main` | Claude Code | In progress | First run started 2026-11-06 05:08 UTC |
| 0.3 | Mixxx bridge: feed every `[ChannelN]`/FX control into the recorder; drive the player from the engine's beat position | Claude Code | Open | Needs the full Mixxx build |
| 0.4 | Rebrand: app name, window title, settings folder, installer name and icon → BeatFreaks | — | Open | Keep settings separate from a real Mixxx install |
| 0.5 | Help menu: Hardware Test screen (S4 MK3 + F1) using `HardwareTestModel` | — | Open | Look: "Hardware Test" board in the screen designs |
| 0.6 | Help menu: "Send log to Claude" using `DiagnosticReport` | — | Open | |

## Stage 1 — DJ mode, rock solid

| # | Task | Owner | State | Notes |
| --- | --- | --- | --- | --- |
| 1.1 | Verify S4 MK3 mapping on Windows (incl. motorized platters, screens) | — | Blocked | Needs mullerm2012 to test on the Windows PC |
| 1.2 | Verify F1 mapping; F1 sample-pad layer for sound effects | — | Open | |
| 1.3 | BeatFreaks skin (edgy look: acid green / hot pink / cyan, Chakra Petch) | — | Open | Based on the "Brand" board. Pow Wow 1: record lanes are the signature; glow + stem waveforms only after Stage 1 hardware sign-off |
| 1.4 | Downloads-folder watch (Beatport / Bandcamp purchases auto-import) | — | Open | |

## Stage 2 — Record + Arrange

| # | Task | Owner | State | Notes |
| --- | --- | --- | --- | --- |
| 2.1 | Arrange view (timeline, clips, lanes) | — | Open | |
| 2.2 | Record pass UI: Touch/Latch, punch-in, armed channels | — | Open | |
| 2.3 | Hybrid mode | — | Open | |

## Stage 3 — Export, streaming + safety

| # | Task | Owner | State | Notes |
| --- | --- | --- | --- | --- |
| 3.1 | Export mix / stems / tracklist | — | Open | |
| 3.2 | OBS WebSocket: now-playing text, scene switches, start/stop stream | DATA | In progress | Plan first (`collab/plans/obs-integration.md`); overlay never shows library or cued decks |
| 3.3 | Stream overlay page for OBS (browser source) | — | Open | |
| 3.4 | Elgato Stream Deck plugin (official SDK) | — | Open | Owner's model: ask mullerm2012 |
| 3.5 | Auto-save and crash recovery | — | Open | |

## Stage 4 — Producer tools

| # | Task | Owner | State | Notes |
| --- | --- | --- | --- | --- |
| 4.1 | Beatmapper, Chopper on F1 pads, loop browser | — | Open | |
| 4.2 | Offline stem splitting | — | Open | |
| 4.3 | Spotify / SoundCloud playlist import (match to owned files) | — | Open | No streaming of their audio; see the plan |
