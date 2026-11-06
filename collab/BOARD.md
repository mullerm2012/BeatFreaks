# BeatFreaks task board

States: **Open** · **In progress** · **In review** · **Blocked** · **Done**.
Claim a task by putting your name in **Owner** and pushing that change
before you start. Stages follow the
[product plan](https://claude.ai/artifact/ReVS2FPEbinKF4BVB4FAFt).

## ★ Owner's #1 priority — Smart Next (said 2026-11-06)

"Search the hard drive for the songs, put them in queue by key and BPM, and
see what will make the crowd move. That's the number one thing."

| # | Task | Owner | State | Notes |
| --- | --- | --- | --- | --- |
| N.1 | Library scan of the owner's music folders (incl. `C:\Users\PukDr\Music`), BPM + key analysis for every track | ALPHA | Open | Mixxx already scans + detects BPM/key; make it automatic on first run and show progress |
| N.2 | **Energy score** per track (0–10) from the audio: loudness, low-end punch, beat density, plus intro/outro sections | ALPHA | In review | Own C++ analyzer, unit-tested; 7.1 (allin1) improves it later |
| N.3 | **Smart Next panel**: while a track plays, rank the library by key match (Camelot), BPM range, energy direction (build / hold / cool down) and genre; one press queues it | ALPHA | In progress | Ranking engine in PR `code/smart-next`; panel UI next | Feeds Mixxx's Auto DJ queue |
| N.4 | **Crowd feedback**: one F1 pad/Stream Deck key "crowd hot" / "crowd cold" while a track plays; BeatFreaks learns what moves the owner's crowds | DATA | Open | Stored per track + venue/crowd type |
| N.5 | Set builder: "1 hour, rising from 100 to 128 BPM, end on crowd movers" → a queue you can edit | — | Open | Uses N.2–N.4; later the AI assistant (7.3) |

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
| 1.4 | Downloads-folder watch (Beatport / Bandcamp purchases auto-import) | — | Open | Default: watch `C:\Users\PukDr\Music` (BPM Supreme dated genre folders), see `IDEAS.md` §5 |
| 1.5 | DJ Badboy MJ macros: one-press **bass-cut blend**; DJ-name tag with echo on an F1 pad | — | Open | Owner's technique, `IDEAS.md` §1 |
| 1.6 | Transition presets "Fast edit" (4-bar) and "Let the music talk" (16-bar); bass/mid/high waveform colours default | — | Open | `IDEAS.md` §1 |
| 1.7 | Artwork: logo, app icon, splash, textures, Stream Deck icons, overlay frame | DATA | Open | `briefs/art-brief.md`; ALPHA integrates |

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
| 3.6 | Mix Map HUD (energy arc) and Set Logger | — | Open | `IDEAS.md` §3 |
| 3.7 | Publish mix to Mixcloud / SoundCloud with tracklist | — | Open | Check APIs/terms first; `IDEAS.md` §6 |

## Stage 4 — Producer tools

| # | Task | Owner | State | Notes |
| --- | --- | --- | --- | --- |
| 4.1 | Beatmapper, Chopper on F1 pads, loop browser | — | Open | |
| 4.2 | Offline stem splitting | — | Open | |
| 4.3 | Spotify / SoundCloud playlist import (match to owned files) | — | Open | No streaming of their audio; see the plan |
| 4.4 | Smart crates (genre, BPM, key, energy, clean/explicit, crowd type) + Transition Coach (top 3 next tracks) | — | Open | `IDEAS.md` §3 |

## Stage 5 — Library anywhere and smart features

| # | Task | Owner | State | Notes |
| --- | --- | --- | --- | --- |
| 5.1 | **BeatFreaks Link**: home PC serves the library; gig laptop pins crates offline; phone/browser page browses and plays it | — | Open | Pow Wow first; `IDEAS.md` §4 |
| 5.2 | Smart next-track picks, auto-transition proposals on the timeline | — | Open | |

## Stage 6 — Studio (beat making, FL-style)

| # | Task | Owner | State | Notes |
| --- | --- | --- | --- | --- |
| 6.0 | Pow Wow 2: Studio engine choice (extend Mixxx vs embed Tracktion Engine) | ALPHA + DATA | Open | `briefs/studio-and-ai.md` |
| 6.1 | Step sequencer patterns as Arrange clips | — | Open | |
| 6.2 | Piano roll + chord/scale tool | — | Open | |
| 6.3 | Sampler + drum kit instruments; F1 pads play them | — | Open | |
| 6.4 | "Remix a song": one click → 4 stem tracks in Arrange | — | Open | Needs 4.2 |
| 6.5 | VST3 instrument hosting | — | Open | |

## Stage 7 — BeatFreaks AI (local, optional)

| # | Task | Owner | State | Notes |
| --- | --- | --- | --- | --- |
| 7.0 | AI Engine helper process + "can this PC run it?" check + fake engine for tests | — | Open | Never on the live audio path |
| 7.1 | AI analysis: beats, bar 1, sections → cue points, Beatmapper, Transition Coach | — | Open | allin1; check license |
| 7.2 | AI generate: loops and full songs at project tempo/key (ACE-Step) | — | Open | MIT/Apache; label outputs as AI |
| 7.3 | AI assistant: set building and mix-out suggestions as editable proposals | — | Open | Small local LLM |
