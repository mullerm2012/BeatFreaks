# 9. Auto DJ

> **Status: Available now** (Mixxx's Auto DJ).
>
> **Coming later:** **Autopilot**, a much smarter BeatFreaks mode that picks
> the next track with Smart Next, finds the right phrase to mix on, and does
> beat-matched transitions with EQ and effects. It is planned only. See the
> end of this chapter.

**Auto DJ** plays a list of tracks one after another and crossfades between
them by itself. Use it for a break, a warm-up, a background playlist, or a
party where nobody is at the decks.

What Auto DJ does: it plays the queue in order, and fades from one deck to the
other using the crossfader. What it does **not** do: it does not choose tracks
by key or energy, and it does not do creative transitions. It is a reliable
jukebox with crossfades.

## 9.1 Open Auto DJ

- Click **Auto DJ** in the library sidebar, or **View → Show Auto DJ**.

The Auto DJ view shows the **queue** (the list of tracks to play) and a row of
buttons.

![Screenshot: Auto DJ view with the queue, Enable button, transition mode and seconds box](images/autodj-view.png)

## 9.2 Fill the queue

Any of these work:

- Drag tracks from the library onto **Auto DJ** in the sidebar, or into the
  queue list.
- Right-click tracks → **Add to Auto DJ Queue (bottom)** / **(top)** /
  **(replace)**.
- Right-click a playlist or crate → **Add to Auto DJ Queue**.
- Set **Preferences → Library → Track Double-Click Action** to "Add track to
  Auto DJ queue" if you build queues a lot.

Change the order by dragging rows. Remove a track: select it and press
**Delete**, or right-click → **Remove**.

The box at the bottom shows the **number and total length** of selected
tracks.

## 9.3 The buttons

| Button | What it does |
| --- | --- |
| **Enable** / **Disable** | Starts or stops Auto DJ |
| **Shuffle** | Mixes up the order of the queue |
| **Skip** | Removes the next track from the queue (skips it) |
| **Fade** | Starts the crossfade to the next track right now |
| **Random** | Adds one random track from the Track Source (see 9.6) |
| **Repeat** | When on, played tracks go back to the end of the queue so it never runs out |
| Transition mode list | How it fades (see 9.4) |
| **sec.** box | Length of the crossfade in seconds |

Keyboard shortcuts (with keyboard shortcuts on): **Shift + F12** enable,
**Shift + F11** fade now, **Shift + F10** skip next, **Shift + F9** shuffle.

## 9.4 Transition modes

| Mode | What happens |
| --- | --- |
| **Full Intro + Outro** | Uses each track's intro and outro markers (chapter 6). It plays the whole outro of the old track over the whole intro of the new one. Best when markers are set well |
| **Fade At Outro Start** | Starts the fade when the old track's outro begins |
| **Full Track** | Plays each track to the end and fades over the set number of seconds |
| **Skip Silence** | Like Full Track, but skips silence at the start and end of tracks |
| **Skip Silence Start Full Volume** | Like Skip Silence, but the new track starts at full volume |

If a track has no intro/outro markers, the marker-based modes fall back to
sensible defaults (the start and end of the track).

Set the **seconds** box to the fade length you want (for example 8–16
seconds for a smooth blend, or a negative number to leave a gap, depending on
mode).

## 9.5 Start Auto DJ

Auto DJ needs a few things to be true:

- Two decks must be on **opposite sides of the crossfader** (deck 1 left,
  deck 2 right; this is the default).
- **At least one** of those decks must be stopped.
- Decks that Auto DJ does not use must be stopped.

Steps:

1. Fill the queue.
2. Stop the decks you are not using.
3. Click **Enable**.
4. Auto DJ loads the first track and plays it, loads the next one on the other
   deck, and crossfades when it is time.
5. The **Auto DJ** light in the toolbar is on while it runs.
6. Click **Disable** to stop. The current track keeps playing; you take over.

If Auto DJ refuses to start, it shows a message such as "Auto DJ requires two
decks assigned to opposite sides of the crossfader" or "One deck must be
stopped to enable Auto DJ mode". Fix that and click **Enable** again.

## 9.6 Track Source and random tracks

Auto DJ can top up the queue on its own from crates you choose.

1. Right-click a crate → tick **Auto DJ Track Source**. (Or open the **Auto
   DJ** item in the sidebar and use its **Crates** list.)
2. Click **Random** to add one random track from those crates.
3. For automatic top-ups, open **Preferences → Auto DJ**:
   - **Enable random track addition to queue**.
   - **Minimum allowed tracks before addition**: when the queue gets this
     short, random tracks are added.
   - **Re-queue Tracks**: **Minimum available tracks in Track Source** (this
     percentage of tracks stays available whatever was played),
     **Suspend track in Track Source from re-queue** and **Suspension period
     for track selection** (how many hours before a played track can come
     back).
   - **Reset the Crossfader back to center after disabling AutoDJ**.

![Screenshot: Preferences, Auto DJ page](images/prefs-autodj.png)

## 9.7 Tips for good Auto DJ sets

- Analyse all tracks first (chapter 4) so BPM, grids and levels are known.
- Turn on **sync lock** (hold SYNC) on both decks if your tracks have similar
  tempos; fades will be beat-matched.
- Set intro/outro markers on your favourite tracks and use **Full Intro +
  Outro** for the smoothest results.
- Keep **ReplayGain** on so every track plays at a similar level.
- Use a crate as Track Source with **Repeat** for an all-night background
  playlist.

## 9.8 Autopilot (Coming later)

Autopilot is BeatFreaks' planned answer to "take a break and let the program
DJ like a real DJ". The plan:

- Pick the next track with **Smart Next** (key, tempo, energy direction, crowd
  reaction, genre; chapter 10).
- Find the right mix points by detecting phrases and sections (intro, build,
  drop, breakdown, outro).
- Choose a transition type (long blend, breakdown-to-breakdown, drop-to-drop,
  loop extension, or a quick echo-out when keys clash).
- Perform it: sync, start on the phrase, bass swap on the bar line, crossfade,
  and an optional filter or echo.
- Record every move it makes as automation lanes, so you can edit them later
  in Arrange mode (chapter 11).
- Hand control back the moment you touch a fader or the platter, and a "Take a
  break" button that hands the set over at the next phrase.

None of this is in the program yet. Today, use Auto DJ as described above.

Next: [Smart Next](10-smart-next.md)
