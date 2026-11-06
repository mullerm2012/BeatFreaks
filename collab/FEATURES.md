# What the other programs have, and BeatFreaks' answer

Research snapshot: 2026-11-06 (Claude Code). The goal is to match the best of
Traktor, Serato, rekordbox, VirtualDJ, djay and ACID, then beat them with
what only BeatFreaks does. **ChatGPT: please check this list, add anything
missing (with a source link) and challenge the priorities in `LOG.md`.**

Legend for **BeatFreaks**: **Mixxx** = already in the Mixxx base, needs
wiring/testing · **Stage N** = planned in that roadmap stage (see
`BOARD.md`) · **Later** = after Stage 4 · **Deal** = needs a business
agreement with another company.

## Only in BeatFreaks (our edge)

| Feature | Why nobody else has it |
| --- | --- |
| Every live DJ move recorded as editable automation lanes on an ACID-style timeline | Traktor DJ Studio once did this and dropped it; DJ.Studio edits mixes offline only ([MusicTech](https://musictech.com/guides/buyers-guide/best-dj-mix-recording-software/)) |
| Hybrid mode: play back an arranged mix and take over any channel live | No DJ app mixes a DAW timeline with live decks |
| Built-in Hardware Test screen and one-click "Send log" report | Others rely on forum support |
| Free and open source, no subscription, no hardware lock-in | Most rivals lock features behind subscriptions or hardware ([rekordbox 7 tiers](https://djmag.com/tech/rekordbox-7-released-alphatheta)) |

## Decks, mixing, FX

| Feature | Who has it | BeatFreaks |
| --- | --- | --- |
| 4 decks, sync, keylock, hotcues, loops, slip, beatjump | All | Mixxx |
| Deck types: track / stem / remix / live input per deck | [Traktor Pro 4](https://www.thomann.de/cz/native_instruments_traktor_pro_4_upgrade.htm) | Mixxx has track + stem decks; remix-deck style via F1 sample layer, Stage 1 |
| Flexible beatgrids for tracks with tempo changes | [Traktor Pro 4](https://djlifemag.com/2024/07/native-instruments-announces-traktor-pro-4-dj-software/), VirtualDJ 2026 AI beatgrid | Stage 4 (Beatmapper also needs a tempo map) |
| Beat-tick to check grids by ear | [Traktor Pro 4](https://community.native-instruments.com/discussion/comment/165542) | Stage 1 (small) |
| Drum pattern player | Traktor Pro 4 (Pattern Player) | Stage 4, alongside loop browser |
| Master maximizer/limiter for loud, clean sets | Traktor Pro 4 (Ozone Maximizer) | Stage 3, open-source limiter on the master |
| Big effects library, hardware-style FX | Traktor 40+ FX; [VirtualDJ 2026 120+ FX](https://www.bonedo.de/?p=2674790) | Mixxx built-ins + VST/AU plugins; BeatFreaks FX pack, Later |
| One-knob mixer FX per channel | Traktor, rekordbox | Mixxx (Quick Effects) |
| DVS (control vinyl) | All majors | Mixxx |

## Stems

| Feature | Who has it | BeatFreaks |
| --- | --- | --- |
| Real-time stem separation during the mix | [VirtualDJ](https://virtualdj.com/products/), Serato, rekordbox, djay | Later (needs an on-device AI model; pre-split stems sound better on big systems) |
| Pre-split stems, 4 coloured stem waveforms | Traktor Pro 4 (RX), DJ.Studio | Stage 4 (Demucs offline); Mixxx 2.6 already plays stem files |
| Stem pads / per-stem FX | VirtualDJ, djay | Mixxx stem controls + F1 layer, Stage 1 |
| Swap one track's stem for another's | VirtualDJ StemSwap | Later; natural in Arrange (drag a stem clip) |

## Library and prep

| Feature | Who has it | BeatFreaks |
| --- | --- | --- |
| Advanced crate search/filter, colour coding, track ratings | [Serato DJ Pro 4](https://de.ra.co/news/83319) | Mixxx partly; polish Stage 1 |
| Related-track suggestions, collection "radar" | [rekordbox 7](https://DJ.Studio/blog/rekordbox-7-announced) | Stage 5 (smart picks) |
| Automatic cue creation | rekordbox 7 (subscription) | Stage 4 |
| Import other apps' libraries, play rekordbox/Serato USBs | rekordbox, VirtualDJ, Mixxx | Mixxx |
| Downloads-folder auto-import | — | Stage 1 |
| Export to USB for CDJs, incl. stems | rekordbox; [VirtualDJ CDJ export](https://djgear2k.com/virtual-dj-software-review/) | Later |

## AI

| Feature | Who has it | BeatFreaks |
| --- | --- | --- |
| Automix with smart transitions | [djay](https://help.algoriddim.com/topic/first-steps/free-vs-pro) | Stage 5, as editable proposals on the timeline |
| AI set building from a text prompt | [VirtualDJ 2026](https://musictech.com/news/gear/virtualdj-2026/) | Stage 5 |
| Lyrics shown in the waveform | VirtualDJ 2026 | Later |
| AI voice-overs / shout-outs | [VirtualDJ 2026](https://www.bonedo.de/artikel/virtualdj-2026-atomix-dj/) | Later (mic lane in Arrange covers real shout-outs now) |

## Video, streaming, recording

| Feature | Who has it | BeatFreaks |
| --- | --- | --- |
| Video mixing, karaoke | VirtualDJ; djay on macOS/iOS | Later |
| Record the mix | All | Core (editable take + audio file) |
| Live broadcast (Icecast/Shoutcast) | VirtualDJ, Mixxx | Mixxx |
| OBS control, stream overlay | — | Stage 3 |
| Elgato Stream Deck plugin | — | Stage 3 |

## Streaming music services

| Service | Who has it | BeatFreaks |
| --- | --- | --- |
| Beatport, SoundCloud, TIDAL | [rekordbox](https://rekordbox.com/en/support/faq/streaming-7), Serato, djay, VirtualDJ | Deal |
| Spotify (personal use only) | [rekordbox, Serato, djay](https://newsroom.spotify.com/tag/dj-software/) | Deal; playlist import of owned files, Stage 4 |
| Apple Music | Serato, djay, rekordbox | Deal |

Note: competitors disable recording with streamed tracks
([djay](https://help.algoriddim.com/hc/en-us/articles/11269681416476-What-is-the-difference-between-the-free-version-and-the-PRO-subscription-of-djay-Pro-for-Windows)),
so recording and livestreaming should use owned files anyway.

## Hardware

| Feature | Who has it | BeatFreaks |
| --- | --- | --- |
| Hundreds of mapped controllers, MIDI learn, scripting | VirtualDJ, Mixxx | Mixxx |
| Controller screens | Traktor (S4 MK3), Mixxx 2.7 | Mixxx, verify Stage 1 |
| Motorized platters (S4 MK3 Haptic Drive) | Traktor; Mixxx (beta) | Mixxx, harden Stage 1 |
