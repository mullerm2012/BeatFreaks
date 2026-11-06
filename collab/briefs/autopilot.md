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
