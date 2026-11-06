# BeatFreaks Studio + AI — proposal (ALPHA, 2026-11-06)

mullerm2012 (2026-11-06): add everything music/DJ software does, including
FL Studio–style beat making and Suno-style AI music creation, plus a small
open-source AI from GitHub that helps DJ and mix. "Make sure it all works."

This is a **proposal for Pow Wow 2**, not a decision. DATA: challenge it.

## 1. What the other tools do

**FL Studio 2026** (Image-Line release notes,
[manual](https://www.image-line.com/support/flstudio_online_manual/html/basics_new.htm)):
Channel Rack + step sequencer for patterns, Piano Roll for notes,
Playlist for arranging, Mixer. 2026 adds a Chord Progression Tool, chord
detection, "Remix a song" (load + stem-separate in one click,
[beta notes](https://forum.image-line.com/viewtopic.php?p=2060425)),
Loop Starter, and an AI assistant (Gopher) that writes piano-roll scripts.

**AI music** (state in 2026):

| Model | Does | License (check before shipping) |
| --- | --- | --- |
| ACE-Step 1.5 / v1 | Full songs with vocals from text + lyrics, LoRA training | MIT / Apache-2.0, outputs usable commercially ([overview](https://www.creativeainews.com/articles/best-open-source-ai-music-models-2026/)) |
| YuE (v1) | Lyrics-to-song | Apache-2.0 (same source) |
| DiffRhythm 2, HeartMuLa | Full songs | Apache-2.0 (same source) |
| Stable Audio Open | Loops, one-shots, sound design, ≤47 s, no vocals | Stability Community License; sources disagree on commercial terms ([VentureBeat](https://venturebeat.com/ai/stability-ai-debuts-new-stable-audio-open-for-sound-design)) — **avoid until checked** |
| MusicGen | Instrumentals | Reported as not usable for selling ([overview](https://www.creativeainews.com/articles/best-open-source-ai-music-models-2026/)) — **avoid** |

Open full-song models run on a single high-end consumer GPU
(RTX 4090 class, [overview](https://www.youngju.dev/blog/culture/2026-05-14-ai-music-generation-2026-suno-udio-riffusion-musicgen-mubert-stable-audio-deep-dive.en)).

**AI for DJing / analysis:** All-In-One structure analyzer (`allin1`):
BPM, beats, downbeats and labelled sections (intro / verse / chorus /
outro) ([PyPI](https://pypi.org/project/allin1/0.0.11/)). Real-time
beat/downbeat trackers such as BeatNet exist
([GitHub topic](https://github.com/topics/beat-tracking)). VirtualDJ 2026
ships an LLM "prompt folder" for track recommendations.

## 2. Proposed shape

Keep BeatFreaks one app with four modes: **DJ · Hybrid · Arrange · Studio**.

**Studio (beat making), built on the Arrange timeline we already have:**
- Pattern lanes: step sequencer (16/32 steps, swing, velocity) whose
  patterns drop onto Arrange tracks as clips.
- Piano roll for MIDI clips; chord tool (scale-aware chords, progressions).
- Instruments: built-in sampler + drum kit first; VST3 instruments later.
  Options to debate: (a) extend Mixxx's engine ourselves; (b) embed
  Tracktion Engine (GPL, timeline/MIDI/plugin hosting, JUCE) only for the
  Studio engine. (b) is far faster to a real DAW but adds JUCE to the build.
- "Remix a song": drop a track, split stems offline, get 4 stem tracks in
  Arrange.
- F1 pads play the sampler/drum kit; S4 platters scrub the timeline.

**BeatFreaks AI (optional download, runs locally):**
- A separate helper process ("AI Engine", Python) the app talks to over
  localhost, so a crash or a missing GPU never touches live audio.
- Jobs, all offline (never during a live set):
  1. **Analyze** (allin1): beatgrid with true bar 1, sections → auto cue
     points, Beatmapper tempo maps, smarter Transition Coach.
  2. **Generate** (ACE-Step): "make me a 124 BPM moombahton intro in 8A",
     lands as a clip in Arrange; loops and full songs with vocals; tempo
     and key locked to the project.
  3. **Stems** (offline separation; licence check needed).
  4. **Assistant** (small local LLM): "build a 1-hour set that rises from
     110 to 128", "where should I mix out?" — answers become editable
     proposals on the timeline, never automatic actions.
- Every AI output is labelled as AI-generated in the project and export
  metadata. No imitation of real artists' voices.

## 3. How we make sure it works

- Every engine piece gets unit tests in the fast core suite; every PR builds
  a Windows installer; owner tests with the checklist.
- AI jobs have a fake engine for tests, and a "can this PC run it?" check
  (GPU/VRAM) before offering a model.
- Studio lands in stages after Stage 2 (Arrange), since it reuses it.

## 4. Open questions for the owner

1. Windows PC graphics card (NVIDIA model + VRAM)? Decides which AI runs
   locally.
2. OK for the AI Engine to be a separate optional download (several GB)?
