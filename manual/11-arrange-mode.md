# 11. Arrange mode

> **Status: In development.** The recording engine for automation lanes and
> the project file format are written and pass their tests. They are **not**
> connected to the decks yet, and there is **no Arrange screen** in the
> program yet. You cannot use Arrange mode today. This chapter explains what
> it will do so you know what is coming.

| Part | Status |
| --- | --- |
| Automation lanes (store moves in beats, smooth or stepped) | In development (engine done and tested) |
| Recorder with Read, Touch and Latch modes, punch-in, armed channels | In development (engine done and tested) |
| Project file (`.bfproj`) with clips, markers and lanes | In development (engine done and tested) |
| Link from the decks, mixer and effects to the recorder | Coming later |
| Arrange screen (timeline, clips, lanes) | Coming later |
| Record-pass controls on screen | Coming later |
| Hybrid mode (DJ decks and timeline together) | Coming later |
| Export mix, stems and tracklist from a project | Coming later |
| Step sequencer, piano roll, sampler instruments, VST3 (Studio) | Coming later |

## 11.1 The idea in one sentence

**Every move you make while DJing (faders, EQ, filter, effects, play, cue,
loops) is recorded as editable lines on a timeline, so you can fix a mix after
you play it, then export it.**

This is the way music production programs (ACID-style arrangers and DAWs,
"digital audio workstations") record knob moves. BeatFreaks brings it to live
DJing.

![Screenshot: (planned) Arrange view with two track clips on a timeline and volume, EQ and filter lanes under them](images/arrange-view-planned.png)

## 11.2 Words used in Arrange mode

| Word | Meaning |
| --- | --- |
| **Timeline** | A horizontal line of time, measured in bars and beats, from the start of your mix to the end |
| **Clip** | One piece of a track placed on the timeline: which deck it is on, where it starts, how long it plays, and which part of the song it starts from. The audio file itself stays where it is on your disk |
| **Lane** | One line of automation for one control, for example "deck 1 volume" or "deck 2 low EQ" |
| **Automation point** | One dot on a lane: a value at a beat. Lines between dots are either smooth ramps (for faders and knobs) or steps (for buttons like play, cue and loop on/off) |
| **Pass** | One recording run |
| **Marker** | A named, coloured spot on the timeline, such as "Bass swap" |
| **Project** | The whole thing, saved as a `.bfproj` file |

Positions are stored in **beats**, not seconds, so moves stay locked to the
music if the tempo changes.

## 11.3 Recording modes (how a new pass treats old moves)

These are standard terms from music production programs.

| Mode | What happens when you touch a control |
| --- | --- |
| **Off** | Nothing is recorded or played back |
| **Read** | Plays back the recorded moves. Touching a control does not record |
| **Touch** | Records while you move a control. When you stop moving it for a short time (default **8 beats = 2 bars**), it glides back to the old recording over a few beats, so there is no jump |
| **Latch** | Records from your first touch until you stop the pass. The last value holds |

Important rule of the recorder: **a pass only changes the lane where you
actually moved something.** Before you touched a control and after you let
go, the old recording is kept exactly as it was.

More options:

- **Punch in / punch out**: only moves inside a chosen range are recorded.
  Everything outside is untouched.
- **Armed channels**: choose which decks record (for example only deck 3).
  If none are chosen, all are armed.
- **Thinning**: after a pass, extra points that do not change the shape are
  removed, so lanes stay clean and easy to edit.
- **Loops and jumps** during a pass are allowed; the recorder follows the
  beat position.

## 11.4 Editing (planned)

When the Arrange screen exists, you will be able to:

- Drag a clip to a new place. The moves recorded on that deck during the clip
  move with it, so a dragged transition keeps its fader and EQ moves.
- Add, move or delete points on a lane.
- Re-record just one fader with Touch mode, leaving everything else alone.
- Add markers.

## 11.5 Project files

- Projects are saved as `.bfproj` files (readable text in JSON format).
- Saving is **atomic**: BeatFreaks writes to a temporary file first and swaps
  it in, so a crash while saving never damages the project.
- A project from a newer BeatFreaks version is refused with a clear message
  instead of being opened wrongly.
- Clips point to your audio files; the audio is not copied into the project.

## 11.6 What to do today

- Record your mixes with **REC** (chapter 8). Mixxx's **Create a CUE file**
  option marks where each track starts.
- Keep **History** (chapter 4) for your tracklists.
- Note: a normal recording (WAV, MP3 and so on) contains only the sound, not
  your fader and knob moves. Mixes recorded before Arrange mode is ready will
  not have editable lanes.

Next: [AI songs](12-ai-songs.md)
