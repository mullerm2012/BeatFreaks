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

## ★ Autopilot — the AI that mixes on its own (said 2026-11-06)

"Mix songs on its own, know where the break is, pick what to play, and do it
how the best DJs do it, so we can take a break." Plan: `briefs/autopilot.md`.

| # | Task | Owner | State | Notes |
| --- | --- | --- | --- | --- |
| A.1 | Phrase/section detector → auto intro/outro cues on 8/16/32-bar boundaries | ALPHA | Open | Built-in fallback first, allin1 later (A.7) |
| A.2 | Transition planner: type (blend / breakdown / drop / loop-extend / echo-out) and start bar | ALPHA | Open | Uses Smart Next + A.1 |
| A.3 | Transition performer: sync, bass swap, crossfade, FX, recorded as automation lanes | ALPHA | Open | Editable in Arrange mode |
| A.4 | Hand-off: touch takes over; "Take a break" button on Stream Deck/F1 | ALPHA + DATA | Open | |
| A.5 | Listening test: 20 transition pairs, owner rates, tune rules | DATA | Open | |
| A.6 | Autopilot UI: next mix point on waveform, bar countdown, type badge | DATA + ALPHA | Open | |
| A.8 | **Transition recipes**: blend, filter rise, echo out, loop roll, double drop, motorized spinback, riser, tempo ramp; saved as presets, recorded as lanes | ALPHA + DATA | Open | Owner: "insanely awesome" |
| A.9 | **FX quality**: click/level/tail/tempo-lock tests in CI for every effect, latency+CPU benchmark, add missing FX (tape stop, riser, beat-masher) | ALPHA (tests) + DATA (sound) | Open | "work better than the other programs" |
| A.10 | **From here to here**: mark a spot in A and a spot in B, Autopilot plans the phrase-correct transition between them | ALPHA | Open | Also drag in Arrange |
| A.11 | **Crowd cam**: webcam motion energy + on-beat motion → floor heat → Smart Next crowd score | ALPHA | Open | Motion only, no faces, nothing saved, off by default |
| A.12 | **AI songs for the floor**: ACE-Step 1.5 (MIT, free, runs locally) built in; plus "Send to Suno" prompt builder + auto-import of downloaded songs; official Suno API if accepted to its partner program | DATA + ALPHA | Open | Owner has no GPU; no unofficial Suno wrappers |
| A.13 | **Tracklist + mix cover** (PR #8): played list, YouTube chapters, CUE, CSV, cover grid | ALPHA | Review | Owner ask 00:45 |
| A.7 | Optional AI pack: allin1 sections/downbeats (license, CPU/GPU) | DATA | Open | Research |

## Mobile — BeatFreaks on the phone (owner, 2026-11-06)

"Can any of this be used on a phone? … connect to it all. Like djay Pro."

| # | Task | Owner | State | Notes |
| --- | --- | --- | --- | --- |
| M.1 | **BeatFreaks Remote**: the PC serves a phone web app over Wi-Fi (scan a QR code, no app store). Library search, Smart Next queue, Autopilot on/off + "take a break", crowd hot/cold, Send to Suno, tracklist | ALPHA | Open | Local network only, pairing code, nothing on the internet |
| M.2 | Phone as **crowd cam** (A.11) and wireless F1-style pads | ALPHA + DATA | Open | Motion only, nothing saved |
| M.3 | Remote look: touch-first BeatFreaks style | DATA | Open | Art brief |
| M.4 | Song requests: the crowd scans a QR, asks for a song, it lands in the DJ's request list (DJ approves) | ALPHA | Open | |
| M.5 | Full BeatFreaks on iPad / Android tablet | — | Later | Mixxx has experimental iOS and Android builds; needs a touch UI; phone audio latency is a risk |

## Cloud — save, sync and play from the cloud (owner, 2026-11-06)

"Save mixes on Google Drive… save and retrieve and play back from the cloud… use all the music platforms."

| # | Task | Owner | State | Notes |
| --- | --- | --- | --- | --- |
| C.1 | **Cloud folder**: recordings, tracklists, covers, `.bfproj`, crates/settings backup auto-saved into a chosen folder (Google Drive for desktop, OneDrive, Dropbox) | ALPHA | Open | No API keys needed; the desktop sync app uploads |
| C.2 | **Play from the cloud**: library can include the Drive folder; "Make available offline" button before a gig; warning if a queued song is cloud-only and the internet is down | ALPHA | Open | Gig safety first |
| C.3 | Direct Google Drive API (upload without the desktop app, share link to a mix) | — | Later | Needs Google OAuth app verification |
| C.4 | Streaming services: **Spotify (back in rekordbox/Serato/djay since Sept 2025)**, Apple Music, TIDAL, Beatport/Beatsource, SoundCloud. All are partner deals per DJ app; owner applies as BeatFreaks, ALPHA writes the pitch and builds | Owner + ALPHA | Later | Spotify DJ use is personal-use only; no unofficial scrapers |
| C.6 | **Streaming plug-in slot**: one `StreamingProvider` interface (login, search, playlists, stream to deck, offline cache rules, per-service limits). Each approved service plugs in without touching the decks, Smart Next or Autopilot | ALPHA | Open | Owner 01:00: "leave it open to all of that" |
| C.5 | Playlist bridge: read the owner's Spotify playlists (names only, Spotify Web API) and match them to songs owned locally; missing ones go on a "to buy" list | ALPHA | Open | Check Spotify developer terms first |

## Hardware — every controller (owner, 2026-11-06 04:20)

"I want it to be able to use all the controllers." BeatFreaks ships Mixxx's 164 built-in mappings (Hercules 25, Numark 20, Pioneer 12, Traktor 11 incl. S4 MK3 + F1, Vestax, Behringer, Reloop, Denon, Novation, Korg, Akai …). Goal: any controller works, the big ones work great.

| # | Task | Owner | State | Notes |
| --- | --- | --- | --- | --- |
| H.1 | **Pioneer DDJ-WeGO4** (owner's) mapping, 4 decks, jog FX, built-in sound card (Pioneer ASIO driver on Windows) | ALPHA | Open | Not built into Mixxx. Build from the owner's own MIDI capture (H.4) + Pioneer's published MIDI list; community WeGO3 code only if its licence allows |
| H.1b | **Numark 4TRAK** (owner's "Numark for Traktor"): 4-deck controller, same hardware as the Numark NS6 with Traktor firmware, plus the 12-knob FX Kommand console | ALPHA | Open | Not built into Mixxx (only NS6II/NS7). Same capture method; NS7/NS6II mappings show Numark's style |
| H.2 | **Missing popular controllers**: research the top sellers not in the list (e.g. DDJ-FLX6/FLX10/REV1/REV7, newer Numark/Hercules/Rane/Denon) and add them, priority by what the owner and his DJ friends use | DATA research, ALPHA builds | Open | Own code only; no copying closed-source mappings |
| H.3 | **"Learn any controller" wizard**: plug in an unknown MIDI controller, BeatFreaks walks through "move the left fader… press play…" and builds a mapping; save and share it | ALPHA | Open | Mixxx has a basic MIDI learn; make it friendly |
| H.4 | **Hardware Test page for every controller**: each button/knob lights up on screen when touched, with a pass/fail log to send | ALPHA | Open | Extends the S4/F1 test page |
| H.5 | BeatFreaks features on every mapping: Smart Next, Autopilot "take a break", crowd hot/cold and record buttons, via a shared helper so each mapping gets them with a few lines | ALPHA | Open | |
| H.6 | Several controllers at once (S4 + F1 + WeGO + Stream Deck) without conflicts | ALPHA | Open | Mixxx supports multiple; test it |
| H.7 | **Full coverage rule** for every BeatFreaks mapping: every button, knob, fader, jog, touch strip, pad and LED on the device does its labelled job, checked against the owner's capture + test page (nothing left unmapped) | ALPHA | Open | Applies to S4 MK3, F1, WeGO4, 4TRAK first |
| H.8 | **Extras on top** (SHIFT layers + spare buttons): Smart Next, Autopilot take-a-break / take-over, crowd hot/cold, record, AI song, transition recipes, Arrange record-arm, OBS scene, F1-style sample pads | ALPHA + DATA (picks layouts) | Open | Owner: "we can add other things too" |
| H.9 | **Button editor**: on screen, tap a control on the drawn controller and pick any action from a list (incl. all BeatFreaks extras); saved per controller; export/import to share | ALPHA | Open | No script writing for the owner |

## Engine 2 — own engine, piece by piece (owner, 2026-11-06 04:45)

Goal: one day BeatFreaks needs no Mixxx code, so the owner can choose its licence. Until the last Mixxx piece is gone, the whole app stays GPL. Releases already made stay GPL forever.

Rules from now on:
- **Our code stays separate:** everything new goes in `src/beatfreaks/` as plain C++/Qt with no Mixxx headers where possible (Smart Next already works this way). That is the code the owner can later reuse in Engine 2.
- **Never copy Mixxx code into `src/beatfreaks/`.** Glue that touches Mixxx (like `analyzerenergy.cpp`) lives outside it and gets rewritten for Engine 2.
- Contributions by ALPHA/DATA are made for mullerm2012 (owner). Have a lawyer confirm ownership/AI-authorship before relicensing.
- Engine 2 pieces are written from our own specs and tests, not by translating Mixxx files.

| # | Piece | Replaces | Notes |
| --- | --- | --- | --- |
| E.1 | Spec + test suite for each piece (what it must do, measured) | — | Write first; DATA can help |
| E.2 | Audio engine: 4 decks, low latency (WASAPI/ASIO), sync, mixer, recording | Mixxx engine | Time-stretch: SoundTouch (LGPL) or a paid library, not Rubber Band (GPL) |
| E.3 | Decoding (Windows Media Foundation / FFmpeg LGPL) and library/scanner | Mixxx sources + library | |
| E.4 | Beat, BPM and key detection | Mixxx analyzers | Energy analysis is already ours |
| E.5 | Controller layer (MIDI/HID) + our own mappings, written from captures | Mixxx controller system + 164 GPL mappings | Uses H.4 capture tool |
| E.6 | UI in BeatFreaks' own design | Mixxx skins | DATA's art is original |
| E.7 | Switch-over: Engine 2 passes the same tests and the owner's hardware tests | — | Only then can the licence change |

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
