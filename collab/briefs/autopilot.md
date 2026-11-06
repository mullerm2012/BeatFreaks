# BeatFreaks Autopilot — the AI that DJs on its own

Owner's ask (2026-11-06): "an AI that can mix songs on its own and knows
where to look for the break… so if we want to take a break it can do it on
its own… look for what to play and how to mix it, and do it how the best
DJs do it."

## What it does

Press **Autopilot** (or a Stream Deck / F1 button). BeatFreaks keeps the set
going: it picks the next track, finds the right spot to mix, beatmatches,
and does the transition with EQ, the way a DJ would. Press it again and you
have the decks back on the next phrase, without a jump.

## The four jobs (what a DJ does, as research describes it)

1. **Pick the next track:** Smart Next (PR #5). Key (Camelot), BPM with
   half/double time, energy direction (build / hold / cool down), crowd
   score, and genre. Autopilot follows an energy curve: warm-up, peak, then
   cool-down.
2. **Find the mix points (the "breaks"):** analyse every track for
   downbeats and sections (intro, build, drop, breakdown, outro). Then mark
   Mixxx intro/outro section cues automatically on 8/16/32-bar phrase
   boundaries. Candidates: allin1 (beats, downbeats, sections; MIT) as an
   optional AI pack, with a built-in fallback that uses the energy curve
   plus the beat grid.
3. **Choose the transition type**, phrase-locked:
   - intro→outro blend (default, 16–32 bars)
   - breakdown→breakdown (energy reset)
   - drop→drop (energy peak)
   - loop-extend when a track has a short outro
   - quick cut / echo-out when keys clash or BPM is too far apart
4. **Perform it:** sync tempo, start the incoming track on its phrase start,
   bass swap on the bar line (cut the new track's lows, swap on the phrase),
   crossfade, and an optional filter or echo. Owner's own style from
   IDEAS §7: 138 BPM club mixes with a bass-cut blend.

## Why ours beats the others

- Every move Autopilot makes is **recorded as automation lanes** on the
  timeline, just like a human take. You can open the mix in Arrange mode
  and fix one fader move.
- **Hand-off both ways:** touching a platter, a fader or a knob during an
  Autopilot transition takes over that control (Touch mode). "Take a break"
  hands the set over at the next phrase.
- It can learn your style: the transition lengths and EQ moves you make by
  hand become Autopilot's defaults (later stage).
- It can include tracks generated in Studio/AI (Pow Wow 2) once they are
  analysed like any other track.

## Building blocks already there

- Mixxx Auto DJ "Full Intro + Outro" and "Fade At Outro Start" modes, which
  use intro/outro section cues to line up and time a crossfade.
- Smart Next engine + energy analyzer (PR #5).
- Automation recorder (lanes, Touch/Latch).

## Research to borrow ideas from (do not copy closed products)

- Vande Veire & De Bie 2018, *From raw audio to a seamless mix* (EURASIP,
  open access, open source): beat/downbeat tracking, cue-point selection,
  EQ and transition types.
- Zehren et al. 2020, *Automatic Detection of Cue Points for DJ Mixing*,
  code at github.com/MZehren/Automix (check the license before reusing it).
- byronxu99/AutoDJ (Mixxx script): harmonic mixing, half/double time, bass
  EQ, entrance/exit cues.

## Tasks (see BOARD.md, section A)

| # | Task | Who |
|---|------|-----|
| A.1 | Phrase/section detector → auto intro/outro cues on 8/16/32-bar boundaries, plus tests | ALPHA |
| A.2 | Transition planner: pick the type and the bar to start, from both tracks' sections + Smart Next | ALPHA |
| A.3 | Transition performer: tempo sync, bass swap, crossfade, filter/echo, written as automation lanes | ALPHA |
| A.4 | Hand-off: platter/fader touch takes over; "Take a break" button; Stream Deck/F1 mapping | ALPHA + DATA |
| A.5 | Listening test set: 20 transition pairs across genres, rated by the owner; tune the rules from the results | DATA |
| A.6 | Autopilot UI: the next mix point on the waveform, a countdown in bars, the transition type badge | DATA (art) + ALPHA |
| A.7 | Optional AI pack: allin1 sections/downbeats (check license, GPU/CPU) | DATA research, ALPHA hook |

## Transition library — "insanely awesome" (owner, 2026-11-06)

Every transition is a **recipe**: a short timeline of automation moves on a
phrase grid. Recipes are saved like presets, so the owner and DATA can make
more and Autopilot picks from them. A recipe can only fire when it fits the
two tracks (key, BPM, sections).

| Recipe | What you hear | When Autopilot uses it |
|---|---|---|
| Long blend + bass swap | 32-bar blend, lows swap on the downbeat | Default, same energy |
| Filter rise | High-pass sweeps up on the outgoing track over 16 bars, new track slams in on the 1 | Building energy |
| Echo out | Echo freeze on the last beat, new track drops clean | Key clash or big BPM jump |
| Loop roll | Loop shrinks 4 → 2 → 1 → ½ → ¼ beat, then the new drop | Into a drop |
| Double drop | Both drops hit on the same 1, then the old one fades | Peak time, matching keys |
| Breakdown swap | New track's breakdown takes over the old one's | Energy reset |
| Spinback / brake | **The S4 MK3 platter physically spins back** on screen and on the deck, and the new track hits | Genre or tempo change, crowd hype |
| Riser + impact | F1 sample riser builds, impact on the drop | Big moments |
| Acapella layer | Vocal from the next track over the outgoing beat (stems) | When stems are available |
| Tempo ramp | Gradual tempo glide across 16–32 bars | Changing BPM range (e.g. 128 → 138) |
| Harmonic lift | Key shift +1 semitone for the next track (energy boost on the Camelot wheel) | Long sets, keep it fresh |

Rules: a transition never starts mid-phrase. FX tails end on a bar line. No
two identical recipes back to back. Every transition is recorded as lanes,
so it can be edited in Arrange mode.

Visuals: the next transition appears on the waveform with a bar countdown,
the recipe name and a preview. Optional OBS scene flash or Stream Deck
light on the drop.

| A.8 | Transition recipe format + editor; first 6 recipes (blend, filter rise, echo out, loop roll, double drop, spinback) | ALPHA (engine) + DATA (sound design, visuals) |

## Owner, 2026-11-06 00:34 — effects, "from here to here", camera, own songs

### FX that beat the others (A.9)
- Every FX gets a test in CI: no clicks, no level jumps or clipping on/off,
  tails end cleanly, and the tempo-synced ones stay locked to the beat grid.
- Benchmark: latency and CPU per FX on the owner's PC, from the hardware
  test page.
- Must-haves: filter, echo/delay (beat-synced, freeze), reverb, flanger,
  phaser, gate/stutter, beat-masher/loop roll, bit crush, brake/spinback,
  noise riser, tape stop. Mixxx already has most; we tune them and add the
  missing ones. All are GPL/our own code, not copied.
- F1 pads and S4 FX knobs are mapped, and each FX move is recorded as a lane.

### "From here to here" (A.10)
Mark any two spots: "this point in deck A" → "that point in deck B"
(a drop, breakdown, vocal or any bar). Autopilot plans a phrase-correct
transition between exactly those spots. If it can't land on the phrase it
says so and suggests the nearest bar that works. You can also drag it on
the Arrange timeline.

### Crowd cam (A.11)
A webcam or phone pointed at the floor. On the PC, BeatFreaks measures
**how much the floor is moving** (motion energy) and whether that motion is
on the beat (motion pulses vs BPM). It turns this into a "floor heat"
score per track, fed into Smart Next's crowd score (N.4) alongside the
manual "crowd hot/cold" pad.

Privacy by design: it only measures motion and does not use face
recognition or identify anyone. Frames stay in memory and are never saved
or uploaded. It is off by default, with a visible "crowd cam on" light. The
owner is responsible for venue rules and notices.

Tech: OpenCV (Apache-2.0) optical flow / frame difference, CPU-cheap,
runs on-device.

### AI songs for the floor (A.12, with Pow Wow 2)
When the floor is cold, Studio/AI (ACE-Step, open license) can generate an
edit or a short tool track in the right key and BPM, based on what the
floor liked tonight. It renders ahead of time in the background and joins
the queue like any track, labelled "AI-made". The owner approves before it
plays (setting), and it needs a capable GPU (open question to the owner).

### A.12 update — owner has no GPU, wants Suno (2026-11-06 00:37)
Suno has no public API yet. In July 2026 it opened an invite-only developer
partner program. Unofficial Suno wrappers break Suno's terms, so
BeatFreaks will not use them.
Plan:
1. **Now: "Send to Suno" button.** BeatFreaks writes the Suno prompt from
   the floor data (style, BPM, key, mood, what worked tonight). It copies
   the prompt and opens suno.com. A watched folder auto-imports the song
   the owner downloads, analyses it and queues it as "AI-made".
2. **Later: direct.** If the owner applies to Suno's partner program and is
   accepted, swap step 1 for the official API.
3. **Backup:** ACE-Step on a rented cloud GPU, or a documented
   music API with commercial terms. Owner decides on cost.
4. **Suno inbox (owner, 00:37: "pull the song or the link"):** watch the
   Downloads folder (and a chosen folder). Any new audio file is imported,
   analysed (BPM/key/energy/sections) and queued in seconds. Pasting a
   Suno song link stores it with the track and opens the page, but the
   owner does the download. Automated downloads from Suno's servers would
   break Suno's terms, so we don't do them. BeatFreaks also writes the
   lyrics + style prompt (our own text) so the owner can paste it in.

### A.12 pick: ACE-Step 1.5 (owner, 00:38: "find an open source one, free")
- MIT license, free for commercial use. The project says it trained only on
  royalty-free material. It does full songs with vocals and lyrics, and
  also covers, repaint and BPM/key detection.
- Runs locally. GPU with under 4 GB VRAM is enough; CPU works but slower.
  It has a REST server (`uv run acestep-api`) that BeatFreaks can call.
- Plan: optional "AI pack" download (~models) that runs ACE-Step 1.5 in
  the background. BeatFreaks sends style + BPM + key + lyrics, and renders
  ahead of time so it's ready before it's needed. On CPU-only PCs it renders
  during quieter moments, and the panel shows the time left.
- Suno stays as the optional "Send to Suno" route for higher quality.
- DATA: measure CPU render time for a 3-minute song on a normal laptop (A.7).
