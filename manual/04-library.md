# 4. The library

> **Status: Available now**

The library is where all your music lives inside BeatFreaks. It never moves or
renames your files. It keeps a list of them, plus everything it learned about
them: BPM, key, cue points, loops, ratings and play counts.

![Screenshot: library with Tracks selected in the sidebar and the track table filled](images/library-tracks.png)

## 4.1 The sidebar

| Item | What it is |
| --- | --- |
| **Tracks** | Every track in your library. The number in brackets is how many. |
| **Auto DJ** | The Auto DJ queue and its controls (chapter 9). |
| **Playlists** | Ordered lists you make, for example "Friday warm-up". |
| **Crates** | Unordered boxes of tracks, for example "Afro house" or "Wedding requests". |
| **Computer** | Browse the folders on your PC and play files that are not in the library yet. |
| **Recordings** | Your recorded mixes (chapter 8). |
| **History** | A list for every session: what you played and when. |
| **Analyze** | Analyse many tracks at once for BPM, key and level. |
| **iTunes**, **Traktor**, **Rekordbox**, **Serato** | Read-only views of other programs' libraries, if they are found on your PC. Turn them on or off in **Preferences → Library → External Libraries** (restart needed). You cannot edit them here, but you can play from them and copy tracks into your own playlists and crates. |

## 4.2 The track table

Each row is one track. Each column is one piece of information.

- **Sort**: click a column title. Click again to reverse.
- **Show or hide columns**: right-click any column title and tick the columns
  you want (for example **BPM**, **Key**, **Genre**, **Rating**, **Played**,
  **Date added**, **Comment**).
- **Resize**: drag the line between two column titles.
- **Move**: drag a column title left or right.
- **Rating**: click the stars.
- **Colour**: right-click a track and pick a **Color** to mark it (shows as a
  coloured bar).

Ways to load a track:

- **Drag** it onto a deck or its waveform.
- **Double-click** it. By default this loads it into the next stopped deck.
  You can change this in **Preferences → Library → Track Double-Click Action**
  (load to deck, add to Auto DJ queue top or bottom, or ignore).
- **Right-click → Load to → Deck 1/2/3/4**, or a sampler or the preview deck.
- On a controller, turn the browse knob and press it (chapter 7).

![Screenshot: right-click menu on a track with Load to, Add to Playlist, Crates and Analyze](images/track-context-menu.png)

### The right-click menu on a track

Main items:

- **Load to**: a deck, sampler or preview deck.
- **Add to Auto DJ Queue (bottom / top / replace)**.
- **Add to Playlist** / **Crates**: add the track to one or more (or **Add to
  New Crate**).
- **Search related Tracks**: find tracks with the same artist, album, genre,
  similar BPM or a compatible key.
- **Analyze**: **Analyze**, **Reanalyze**, **Reanalyze (constant BPM)** or
  **Reanalyze (variable BPM)**.
- **Metadata**: **Import From File Tags**, **Import From MusicBrainz**,
  **Export To File Tags**.
- **Adjust BPM**: **Lock BPM** / **Unlock BPM**, **Double BPM**, **Halve BPM**,
  **2/3**, **3/4**, **4/5**, **5/4**, **4/3**, **3/2 BPM**, **Shift Beatgrid
  Half Beat**, **Undo last BPM/beats change**, **Clear BPM and Beatgrid**.
- **Select Color**, rating.
- **Clear**: wipe one kind of information for this track, such as **Play
  Count**, **BPM and Beatgrid**, **Rating**, **Comment**, **Cue Point**,
  **Hotcues**, **Intro**, **Outro**, **Loops**, **ReplayGain**, **Waveform**,
  **Key** or **Cover Art**. Useful if analysis went wrong.
- **Update ReplayGain from Deck Gain**: store the level you set by ear on the
  deck's gain knob.
- **Properties**: open the **Track Editor** (tags, BPM, key, comments).
- **Open in File Browser**: show the file in Windows.
- **Select in Library**, **Find on Web**.
- **Remove from Playlist** / **Remove from Crate**, or on the Tracks view
  **Hide from Library** / **Purge from Library**. Hiding or purging does
  **not** delete the file from your disk.
- **Move Track File(s) to Trash**: this one **does** delete the file (to the
  Windows Recycle Bin). BeatFreaks asks first.

## 4.3 Searching

Click the search box (or press **Ctrl + F**) and type. The table filters as you
type. Press **Esc** to clear it.

Simple search:

- `daft punk` finds tracks where any word matches artist, title, album and so
  on.

Search by field (type the field name, a colon, and the value):

| You type | You get |
| --- | --- |
| `artist:drake` or `a:drake` | Artist contains "drake" |
| `title:remix` or `t:remix` | Title contains "remix" |
| `genre:house` or `g:house` | Genre contains "house" |
| `album:"greatest hits"` | Quotes keep words together |
| `crate:wedding` | Tracks in a crate called "wedding" |
| `bpm:128` | Exactly 128 BPM (rounded) |
| `bpm:120-128` | From 120 to 128 BPM |
| `bpm:>125` | Faster than 125 BPM |
| `~bpm:124` | Close to 124 BPM, within your tempo slider range (the range is set in **Preferences → Library → Track Search**) |
| `key:8A` | Tracks in key 8A |
| `~key:8A` | Tracks whose key mixes well with 8A |
| `rating:>=4` | Four or five stars |
| `played:0` | Never played |
| `year:1999` | Year 1999 |
| `-genre:pop` | A minus sign means "not": everything that is not pop |
| `house | techno` | Either word (OR) |

Combine them: `genre:house ~key:8A bpm:120-126` finds house tracks that mix
well with 8A between 120 and 126 BPM. This is a good way to find your next
track by hand today, before Smart Next is ready (chapter 10).

To search everything while you are inside a playlist or crate, use **Library →
Search in Tracks Library…** (Ctrl + Shift + F).

![Screenshot: search box with "genre:house ~key:8A bpm:120-126" and the filtered results](images/library-search-filters.png)

## 4.4 Crates

A **crate** is a box of tracks with no fixed order. A track can be in many
crates. Use crates to sort by style, mood or event.

Make a crate:

1. Click **Library → Create New Crate** (Ctrl + Shift + N), or right-click
   **Crates** in the sidebar and click **Create New Crate**.
2. Type a name and click **OK**.

Add tracks:

- Drag tracks from the table onto the crate in the sidebar, or
- Right-click tracks → **Crates** → tick the crate.

Right-click a crate for more: **Rename**, **Duplicate**, **Lock** (protects it
from changes), **Remove**, **Analyze entire Crate**, **Auto DJ Track Source**
(lets Auto DJ pick random tracks from it), **Import Crate** and **Export
Crate** (M3U, M3U8, PLS, CSV or TXT), and **Export Track Files** (copies the
audio files to a folder, for example a USB stick).

## 4.5 Playlists

A **playlist** keeps tracks in an order you choose. Use it to plan a set.

Make one:

1. Click **Library → Create New Playlist** (Ctrl + N), or right-click
   **Playlists** → **Create New Playlist**.
2. Type a name and click **OK**.

Add tracks by dragging them onto the playlist, or right-click → **Add to
Playlist**.

Change the order: open the playlist and drag rows up or down, or sort by a
column and right-click the playlist → **Adopt current order**.

Right-click a playlist for: **Rename**, **Duplicate**, **Lock**, **Remove**,
**Shuffle Playlist**, **Analyze entire Playlist**, **Add to Auto DJ Queue**
(bottom, top or replace), **Import Playlist**, **Export Playlist**, **Export
Track Files**, and **Export to Engine DJ**.

> Playlists are great for planning, but a crowd rarely follows your plan. Keep
> a crate of "safe" tracks ready as well.

## 4.6 History

Every time you start BeatFreaks, a new **History** list begins. Each track you
play is added with the time it started.

- Click **History** in the sidebar, then a date, to see what you played.
- Right-click a history list for **Finish current and start new** (split a
  long session), **Join with previous**, **Lock**, **Export Playlist**, and
  **Mark all tracks played**.
- **Preferences → Library → Session History** controls how often the same track
  is logged again and deletes very short lists automatically.

History is the raw list that the BeatFreaks tracklist tools will build on
(chapter 13).

![Screenshot: History in the sidebar with a dated list open](images/library-history.png)

## 4.7 Computer (browse folders)

Click **Computer** to browse your drives. You can:

- Play a file straight away by dragging it to a deck.
- Right-click a folder → **Add to Library** to add it to your music folders.

## 4.8 Analyse tracks before a gig

Analysis works out each track's **beat grid** (where each beat is), **BPM**,
**key**, **ReplayGain** (loudness level) and, in BeatFreaks, the **energy
score** (stored but not shown yet). Tracks are analysed when first loaded, but
analysing in advance means no waiting during a set.

1. Click **Analyze** in the sidebar.
2. Click **New** to see tracks added in the last few days (change the number of
   **days** if you like), or **All** to see everything.
3. Click **Select All**, or select the tracks you want.
4. Click **Analyze**.
5. Watch **Progress**. You can keep using BeatFreaks while it works.

![Screenshot: Analyze view with New and All buttons, Select All and Analyze](images/analyze-view.png)

You can also right-click a playlist or crate → **Analyze entire Playlist** /
**Analyze entire Crate**.

## 4.9 BPM, beat grid and key

### Beat detection settings

**Preferences → Beat Detection**:

- **Enable BPM and Beat Detection**: keep this on.
- **Choose Analyzer**: the method. Keep the default unless a style of music is
  often wrong.
- **Assume constant tempo (Recommended)**: best for electronic and most club
  music. Turn it off only for live-drummed music whose tempo drifts.
- **Enable Fast Analysis**: faster on slow PCs, slightly less accurate.
- **Re-analyze beats when settings change…** and **Re-analyze beatgrids
  imported from other DJ software**: tick these if you want BeatFreaks to
  redo grids made by other programs.

### Fixing a wrong BPM

Sometimes a track is detected at half or double speed (for example 70 instead
of 140), or the grid is slightly off.

- **Half or double**: right-click the track → **Adjust BPM** → **Double BPM**
  or **Halve BPM**. Or open **Properties**, click the **BPM** tab and use
  **Halve BPM**, **Double BPM**, **2/3 BPM**, **3/2 BPM** and so on. Click
  **Apply**.
- **Tap the tempo**: in **Properties → BPM**, click **Tap to Beat** in time
  with the music. (With keyboard shortcuts on and a US keyboard, **Shift + 1**
  taps deck 1's BPM and **Shift + 6** deck 2's.)
- **Grid starts in the wrong place**: play the track, and when a strong beat
  (for example the first kick) is under the play marker, click the deck's
  **Adjust Beatgrid** button. The nearest grid line moves to that spot.
- **Lock it**: once correct, right-click the track → **Adjust BPM** → **Lock
  BPM**, so it is not changed by re-analysis.

Also see chapter 15, section 15.6.

### Key detection and notation

**Preferences → Key Detection**:

- **Enable Key Detection**: keep on.
- **Key Notation**: how keys are written. Options:
  - **Lancelot** (for example `8A`, `9B`). This uses the same numbers and
    letters as the "Camelot wheel" many DJs know: number 1–12 around the
    wheel, **A** = minor, **B** = major. **Choose this if you mix by key with
    Camelot numbers.** Smart Next also uses this system (chapter 10).
  - **OpenKey** (for example `1m`, `1d`): another numbered wheel.
  - **Traditional** (for example `Am`, `C`): the musical name.
  - **Lancelot/Traditional** and **OpenKey/Traditional**: both at once.
  - **Custom**: type your own names.
- **Enable Fast Analysis**: faster, less accurate.

**View → Show Keywheel** shows a key wheel on screen.

![Screenshot: Key Detection preferences with Lancelot notation selected](images/prefs-key-notation.png)
![Screenshot: Keywheel window](images/keywheel.png)

## 4.10 Track properties

Right-click a track → **Properties**. The **Track Editor** opens. You can edit:

- **Summary** tab: Title, Artist, Album, Album Artist, Composer, Year, Genre,
  Track #, Grouping, Key, Comments and Color. It also shows Duration, file
  type, Bitrate, ReplayGain, sample rate, Date added, file size and Location.
- **BPM** tab: the BPM, the lock, **Assume constant tempo**, the scale buttons
  (**Halve BPM**, **Double BPM**, **2/3**, **3/2**, **3/4**, **4/3**, **4/5**,
  **5/4 BPM**), **Tap to Beat** and **Clear BPM and Beatgrid**.
- Cover art (right-click the picture to change it).

Buttons: **Previous** / **Next** (move through the list), **Import Metadata
from MusicBrainz**, **Re-Import Metadata from file**, **Open in File
Browser**, **Apply**, **OK**, **Cancel**.

Edits stay in the BeatFreaks library. To also write them into the music file's
tags, turn on **Preferences → Library → Synchronize library track metadata
from/to file tags**.

![Screenshot: track Properties window](images/track-properties.png)

## 4.11 Library preferences worth knowing

**Preferences → Library**:

- **Music Directories**: add, remove or relink folders (chapter 2).
- **Rescan directories on start-up**.
- **Track Double-Click Action**.
- **Library Row Height** and **Library Font**: make the list easier to read in
  a dark club.
- **BPM display precision**: how many decimals.
- **Grey out played tracks**: tracks you already played this session turn
  grey. Very helpful.
- **Search-as-you-type timeout**, **Enable search completions**, **Enable
  search history keyboard shortcuts**.
- **Percentage of pitch slider range for 'fuzzy' BPM search** (used by
  `~bpm:`).
- **Session History** options.
- **External Libraries** on/off.
- **Preferred Cover Art Fetcher Resolution** (for MusicBrainz cover art).
- **Open Mixxx Settings Folder** button (the same as **Help → Settings
  directory**).

Next: [Decks and mixing](05-decks-and-mixing.md)
