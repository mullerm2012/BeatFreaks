# 10. Smart Next

> **Status: In development.** The engine that ranks tracks and the energy
> analysis are written and tested. The energy score is already measured for
> each track when it is analysed. But there is **no Smart Next panel or button
> in the program yet**, so you cannot use it today. This chapter explains how
> it works and how to get the same result by hand until the panel is ready.

## 10.1 What Smart Next is for

While a track plays, Smart Next looks through your library and answers one
question: **"What should I play next?"**

It ranks tracks by how well they fit:

1. **Key**: will the two tracks sound good together?
2. **Tempo (BPM)**: can you mix them without a big speed change? Half-time
   and double-time count too.
3. **Energy direction**: do you want to **Build** (take it higher), **Hold**
   (keep the vibe) or **Cool down** (bring it down)?
4. **Crowd score**: did this track move the crowd before?
5. **Genre**: a small bonus for staying in the same genre.

When the panel is built, you will see a short list (10 tracks by default)
with a score and a reason for each, for example:

`Key 8A→9A · +1.6% · energy ↑ · crowd mover`

One press will queue the track (it will feed Mixxx's Auto DJ queue).

![Screenshot: (planned) Smart Next panel with a ranked list, scores and reasons, and Build / Hold / Cool down buttons](images/smart-next-panel-planned.png)

## 10.2 Key matching (the Camelot wheel)

Smart Next uses the **Camelot wheel**: 12 numbers around a circle, each with
an **A** (minor key) and a **B** (major key). BeatFreaks shows keys this way
when you choose the **Lancelot** key notation (chapter 4, section 4.9).

How Smart Next scores a key change (from the playing track to the
candidate):

| Move | Example | Score |
| --- | --- | --- |
| Same key | 8A → 8A | Best (100%) |
| One step around the wheel | 8A → 7A or 9A | Very good (90%) |
| Same number, other letter (relative major/minor) | 8A → 8B | Very good (90%) |
| Two steps around the wheel (up is the classic "energy boost") | 8A → 10A or 6A | OK (60%) |
| Diagonal move (one step and switch letter) | 8A → 9B or 7B | Works with care (50%) |
| Key unknown (not analysed) | — | Neutral (50%) |
| Anything else | 8A → 3B | Clash (0%) |

## 10.3 Tempo matching

- The default limit is a **6% tempo change**. Tracks that would need more are
  left out.
- If **half/double time** is allowed (on by default), Smart Next also tries
  playing the candidate at double or half speed. A 70 BPM track can follow a
  140 BPM track.
- The smaller the change needed, the higher the score.
- If either track has no BPM yet, the tempo part counts as neutral.

## 10.4 Energy

Each track gets an **energy score from 0 to 10**:

- 0 = ambient, very calm.
- 10 = peak-time banger.

It is measured from the audio during analysis, from three things:

- **Loudness** (how loud the track is on average) – the biggest part.
- **Low-end punch** (how much of the sound is bass and kick).
- **Beat density** (how busy the rhythm is).

This measurement already runs whenever BeatFreaks analyses a track (chapter
4), and the result is saved in your library. There is no column to see it yet.

How each direction scores the candidate's energy compared with the playing
track:

| Direction | Best match |
| --- | --- |
| **Build** | About 0.5 to 2 points **higher**. Lower energy scores poorly |
| **Hold** | About the **same** energy |
| **Cool down** | About 0.5 to 2 points **lower**. Higher energy scores poorly |

## 10.5 Crowd score

The plan is a **"crowd hot" / "crowd cold"** button (for example an F1 pad or
a Stream Deck key) that you press while a track plays. BeatFreaks learns which
tracks move your crowds. Smart Next already has room for this score (from -1
to +1); tracks with a high score are marked "crowd mover".

> The crowd buttons are **Coming later**. Until then every track's crowd
> score is neutral.

An optional **crowd cam** (the webcam measures movement on the floor, motion
only, no faces, nothing saved, off by default) is also planned. **Coming
later.**

## 10.6 How the final score is made

Each part gives a value from 0 to 1, then they are weighted:

| Part | Weight |
| --- | --- |
| Key | 32% |
| Tempo | 25% |
| Energy direction | 23% |
| Crowd score | 12% |
| Same genre | 8% |

The total is shown as 0–100.

Extra rules:

- The track that is playing is never suggested.
- Tracks you already played tonight go to the **bottom** of the list. They only
  appear if nothing else fits.
- If two tracks have the same score, they are sorted by title, so the list
  does not jump around.

## 10.7 Do it by hand today

You can get very close to Smart Next with the library search (chapter 4):

1. Set **Key Notation** to **Lancelot** (Preferences → Key Detection).
2. Analyse your library so every track has a BPM and key.
3. While a track plays, note its key and BPM, for example **8A** and **124**.
4. In the search box type: `~key:8A ~bpm:124`
   - `~key:8A` finds keys that mix well with 8A.
   - `~bpm:124` finds tempos close to 124 (within your tempo slider range).
5. Add `genre:house` (or your genre) to stay in style.
6. Sort by **Rating** or **Played** to find proven tracks.
7. Turn on **Preferences → Library → Grey out played tracks** so you do not
   repeat yourself.
8. You can also right-click the playing track → **Search related Tracks** →
   **Key** ("harmonic with …") or **BPM**.

## 10.8 Related plans (Coming later)

- **Set builder**: "1 hour, rising from 100 to 128 BPM, end on crowd movers"
  turns into a queue you can edit.
- **Smart crates**: crates that fill themselves by genre, BPM, key, energy,
  clean/explicit and crowd type.
- **Energy arc display** (Mix Map) showing how your set rises and falls.
- **Autopilot** (chapter 9) will use Smart Next to choose its tracks.
- **Automatic first-run scan** of your Music folder with BPM and key analysis
  and a progress display.

Next: [Arrange mode](11-arrange-mode.md)
