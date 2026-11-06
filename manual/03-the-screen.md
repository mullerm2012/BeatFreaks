# 3. The screen

> **Status: Available now.** BeatFreaks currently uses Mixxx's standard
> **LateNight** look (called a **skin**). A BeatFreaks skin with its own
> colours and fonts is **Coming later**. This chapter describes LateNight.

The main window has seven areas, from top to bottom:

1. Menu bar
2. Toolbar
3. Waveforms
4. Decks (left and right) with the mixer in the middle
5. Effects, samplers and microphone/aux rows (can be hidden)
6. Library
7. Status messages (pop-ups when something needs your attention)

![Screenshot: main window with each of the areas numbered 1 to 6](images/screen-areas-numbered.png)

Tip: hover the mouse over any button and wait a moment. A **tooltip** (small
help box) explains what it does, including what a right-click does.

## 3.1 Menu bar

| Menu | What is inside |
| --- | --- |
| **File** | **Load Track to Deck 1/2/3/4** (open a file from disk straight into a deck), **Exit**. |
| **Library** | **Rescan Library**, **Export Library to Engine DJ**, **Search in Current View…** (Ctrl+F), **Search in Tracks Library…** (Ctrl+Shift+F), **Create New Playlist** (Ctrl+N), **Create New Crate** (Ctrl+Shift+N). |
| **View** | **Auto-hide menu bar**, **Show Skin Settings Menu** (Ctrl+1), **Show Microphone Section** (Ctrl+2), **Show Vinyl Control Section** (Ctrl+3), **Show Preview Deck** (Ctrl+4), **Show Cover Art** (Ctrl+6), **Show Keywheel**, **Maximize Library** (Space), **Show Auto DJ**, **Full Screen** (F11). |
| **Options** | **Vinyl Control** (turn timecode vinyl on per deck), **Record Mix** (Ctrl+R), **Enable Live Broadcasting** (Ctrl+L), **Enable Keyboard Shortcuts**, **Preferences** (Ctrl+P). |
| **Developer** | Tools for people who make skins and mappings. You do not need it for DJing. It only appears when BeatFreaks is started in developer mode. |
| **Help** | **Community Support**, **User Manual** (opens the Mixxx manual, which covers the Mixxx parts of BeatFreaks), **Keyboard Shortcuts**, **Settings directory** (opens your settings folder, see chapter 14), **Translate This Application**, **About**. |

> The planned BeatFreaks items **Help → Hardware Test** and **Help → Send
> log** are **In development**. They are not in the menu yet.

![Screenshot: View menu open](images/menu-view.png)

## 3.2 Toolbar

The thin bar under the menu. From left to right:

| Item | What it does |
| --- | --- |
| **BIG LIBRARY** | Makes the library fill most of the window. Good for finding tracks. Click again to go back. (Same as **View → Maximize Library**.) |
| **WAVEFORMS** | Shows or hides the big scrolling waveforms. |
| **4 DECKS** | Switches between 2 and 4 decks on screen. |
| **MIXER** | Shows or hides the mixer in the middle. |
| **EFFECTS** | Shows or hides the effect units. |
| **SAMPLERS** | Shows or hides the sampler decks. |
| **MIC/AUX** | Shows or hides microphone and auxiliary input controls. |
| Auto DJ light | Lights up when Auto DJ is running (chapter 9). |
| Clock | The current time. |
| **Buffer %** meter | How busy the audio engine is. If it reaches the red end you may hear clicks. See chapter 15. |
| Battery | Laptop battery level (laptops only). |
| **REC** | Starts and stops recording your mix. While recording it shows the elapsed time (chapter 8). |
| **ON AIR** | Starts and stops live broadcasting. Shows **ERROR** if the connection fails (chapter 8). |
| **SETTINGS** | Opens the skin settings panel on the right (see 3.8). |

![Screenshot: toolbar with every button labelled](images/toolbar-labelled.png)

## 3.3 Waveforms

The large coloured bands that scroll from right to left. There is one per
deck.

- The **play marker** is the line in the middle. What is under it is what you
  hear.
- **Colours** show the sound: low bass, mids and highs have different
  colours.
- **Beat grid**: thin vertical lines on each beat. Thicker lines mark the start
  of each bar (every 4 beats).
- **Markers**: the cue point, hotcues (coloured, numbered), loop in/out
  (a shaded area), and intro/outro markers.
- **Zoom**: scroll the mouse wheel over a waveform to zoom in and out.
- **Scratch**: click and drag the waveform to move the track (like touching a
  record).

![Screenshot: two scrolling waveforms with play marker, beat grid, hotcues and a loop marked](images/waveforms-labelled.png)

## 3.4 A deck

Each deck plays one track. Decks 1 and 3 are on the left, 2 and 4 on the
right. Default deck colours on many controllers: deck A red, B blue, C yellow,
D purple.

Top of the deck:

- **Cover art** (if the file has a picture).
- **Title** and **Artist**.
- **Time**: elapsed and/or remaining. Click it to switch. The format is set in
  **Preferences → Decks → Time Format**.
- **BPM**: the current tempo. It changes when you move the tempo slider.
- **Key**: the current musical key (see chapter 4, section 4.9, for the key
  notation).
- **Overview waveform**: the whole track, small. Click anywhere on it to jump
  there. The part already played is shaded.

Transport and cue buttons (bottom left of each deck):

- **Play/Pause** (triangle). Right-click sets the cue point.
- **Cue**. See chapter 5 for how it behaves.
- **Hotcue buttons** (numbered). See chapter 6.
- **Intro/outro** marker buttons (can be shown with skin settings).

Loop and jump buttons:

- **Beatloop** button with a size (for example 4). Click to loop that many
  beats.
- **Halve / double** arrows next to the loop size.
- **Loop in**, **Loop out**, **Reloop** (loop on/off).
- **Beat jump** back and forward, with its own size.

Tempo and key (on the right of the left deck, the left of the right deck):

- **Tempo slider** (also called pitch fader). Up or down changes the speed.
- **Tempo nudge** buttons: hold to speed up or slow down for a moment (to fix
  drift by ear).
- **SYNC** button. See chapter 5.
- **Sync leader** button (crown symbol). Chooses which deck the others follow.
- **Key lock** button (lock symbol). Keeps the key the same when you change
  the tempo.
- **Key** buttons: move the key up or down, **Sync key**, and **Reset key**.

Other buttons (some only appear in larger deck sizes):

- **Eject**: unloads the track. Clicking eject on an empty deck reloads the
  last ejected track.
- **Repeat**: plays the track again from the start when it ends.
- **Slip**: while on, loops and scratches do not move the track forward; when
  you let go, the music continues where it would have been.
- **Quantize**: makes cues, loops and jumps snap to the nearest beat.
- **Reverse**: plays backwards.
- **Beat grid adjust** buttons: fix a beat grid that is off (chapter 4,
  section 4.9).

![Screenshot: one deck with every button labelled](images/deck-labelled.png)

## 3.5 Mixer

The mixer sits between the decks. Each deck has a column:

- **Gain** knob (top): sets the input level so all tracks are about equally
  loud. BeatFreaks also auto-levels tracks (ReplayGain) during analysis.
- **EQ** knobs: **High**, **Mid**, **Low**. Turn left to cut, right to boost.
  Double-click a knob to reset it to the middle.
- **Kill** buttons (if shown in skin settings): cut a band completely.
- **Quick effect** knob (also called the filter or super knob): by default a
  filter. Left = cut highs (low-pass), right = cut lows (high-pass). Middle =
  off.
- **Headphone (PFL)** button: send this deck to your headphones.
- **Channel volume** fader.
- **Level meter**: lights up with the volume. Stay out of the red.
- **Crossfader orientation**: decides whether the deck is on the left side,
  right side or centre of the crossfader.

The middle section has:

- **Main** volume and level meters.
- **Headphone volume** and **Headphone mix** (how much of the main mix you
  hear in headphones next to the cue). Also **Split cue** to hear the cue in
  one ear and the main mix in the other.
- **Booth** volume (if a booth output is set up).
- **Crossfader** at the bottom.

![Screenshot: mixer section with each knob and fader labelled](images/mixer-labelled.png)

## 3.6 Effects, samplers, mic/aux

These rows appear when you turn them on in the toolbar.

- **Effect units**: each has three effect slots, a **Mix** knob (dry/wet) and
  a **Super** knob. Buttons show which decks the unit is assigned to. See
  chapter 6.
- **Samplers**: small extra decks for short sounds (air horn, drop, vocal
  shout). Drag a file onto a sampler and click it to play.
- **Mic/Aux**: talk-over button and volume for microphones; volume and
  crossfader setting for auxiliary inputs.

![Screenshot: effect rack with two units, three effects each](images/effects-rack.png)
![Screenshot: sampler row with eight samplers, two of them loaded](images/samplers-row.png)

## 3.7 Library

The bottom part of the window. Details are in chapter 4.

- **Sidebar** (left): Tracks, Auto DJ, Playlists, Crates, Computer,
  Recordings, History, Analyze, plus other DJ programs' libraries (iTunes,
  Traktor, Rekordbox, Serato) if found.
- **Search box** (top): type to filter.
- **Track table** (right): your tracks with columns such as Artist, Title,
  BPM, Key, Duration, Rating and more.
- **Preview deck** (optional, **View → Show Preview Deck**): a small player to
  listen in your headphones without using a main deck.

![Screenshot: library with sidebar, search box and track table](images/library-overview.png)

## 3.8 Skin settings panel

Click **SETTINGS** in the toolbar (or **View → Show Skin Settings Menu**). A
panel opens on the right. Useful options include:

- **Decks**: deck size (for example **Compact**, **mini**, **FULL**), 2 or 4
  decks.
- **Mixer**: show **EQ Knobs**, **EQ Kill Buttons**, **Crossfader**, **Super
  Knobs**, **Main / Headphone Mixer**.
- **Effect Units**: 2 or 4 units.
- **Cover Art**, **Preview Deck**, **Hotcue Shift Buttons**, **Waveforms**.
- Library options.

Click **SETTINGS** again to close the panel.

![Screenshot: skin settings panel open on the right side](images/skin-settings-panel.png)

## 3.9 Preferences window

Click **Options → Preferences** (Ctrl+P). The pages on the left:

| Page | What you set there | Chapter |
| --- | --- | --- |
| **Sound Hardware** | Sound card, outputs, latency | 2 |
| **Library** | Music folders, how double-click works, row height, font, history settings, other programs' libraries, settings folder button | 4 |
| **Controllers** | Turn controllers on, choose mappings, mapping settings | 7 |
| **Vinyl Control** | Timecode vinyl | — |
| **Interface** | Skin, colour scheme, language, screen scaling, screensaver | 3 |
| **Waveforms** | Waveform type, frame rate, zoom, end-of-track warning time, beat grid look | 3 |
| **Colors** | Hotcue and track colour palettes | 6 |
| **Decks** | Cue mode, time format, track load point, speed slider range, key lock behaviour, sync options | 5 |
| **Mixer** | EQ type, crossfader curve | 5 |
| **Effects** | Which effects show in lists, quick effect presets | 6 |
| **Auto DJ** | Re-queue and random track rules | 9 |
| **Live Broadcasting** | Server connections | 8 |
| **Recording** | Folder, format, CUE file | 8 |
| **Beat Detection** | BPM analyser options | 4 |
| **Key Detection** | Key analyser and key notation | 4 |
| **Normalization** | ReplayGain (auto level) | 5 |
| **Modplug Decoder** | Settings for tracker music files (rarely needed) | — |

Buttons at the bottom: **Help**, **Restore Defaults** (for the page you are
on), **Apply**, **Cancel**, **OK**.

![Screenshot: Preferences window with the page list on the left](images/preferences-window.png)

## 3.10 Changing the look

1. Click **Options → Preferences → Interface**.
2. Choose a **Skin**: **LateNight** (default), **Deere**, **Shade** or
   **Tango**. Each has versions with 64 samplers.
3. Some skins have colour schemes (LateNight has **PaleMoon** and
   **Classic**).
4. Click **OK**.

Next: [The library](04-library.md)
