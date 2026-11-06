# 16. FAQ and glossary

## 16.1 Frequently asked questions

**Is BeatFreaks free?**
BeatFreaks is built on Mixxx and is licensed under the GNU GPL (chapter 17).
Test versions are shared by the BeatFreaks team. A paid version with a
one-time price (no monthly fee) is planned; details are not final.

**Is BeatFreaks the same as Mixxx?**
It contains all of Mixxx and adds BeatFreaks features on top. It is a separate
program, not made or supported by the Mixxx project.

**Can I use BeatFreaks and Mixxx on the same PC?**
Yes, with a BeatFreaks-named installer: it keeps its own program folder and
its own settings folder. Very early test installers still named "Mixxx"
replaced Mixxx (chapter 1).

**Does it work on Mac or Linux?**
BeatFreaks is made for Windows 10 and 11 (64-bit). Mac and Linux versions are
not offered.

**Does it work on my phone or tablet?**
Not yet. A **phone remote** (a web page on your phone, served by your PC over
Wi-Fi: library search, Smart Next queue, crowd buttons, song requests) is
**Coming later**. A full tablet version is a later idea.

**Can I use Spotify, Apple Music, TIDAL, Beatport, Beatsource or SoundCloud
streaming?**
No, not now. Each service needs a partner agreement with the DJ program.
BeatFreaks has no such agreements yet. Support is **Coming later** and only
through official partner routes. Use music files you own.

**Can I import my Traktor, Rekordbox or Serato library?**
You can **see and play** them: they appear in the library sidebar (read-only)
if found on your PC. Turn them on in **Preferences → Library → External
Libraries**. You can copy tracks from them into your own crates and playlists.

**Which controllers work?**
Every controller with one of the 164 built-in mappings, including the Traktor
Kontrol S4 MK3 and F1 (chapter 7). MIDI controllers without a mapping can be
taught with the Learning Wizard. The Pioneer DDJ-WeGO4 and Numark 4TRAK are
**Coming later**.

**Do the S4 MK3's motorised platters work?**
The mapping supports them as a **BETA** option that is **off by default**
(chapter 7). BeatFreaks testing on Windows is still going on.

**Does Smart Next work?**
Not from the program yet. The engine is ready and tested; the panel is being
built (chapter 10). Use the `~key:` and `~bpm:` searches in the meantime.

**Can BeatFreaks mix by itself?**
**Auto DJ** (available now) plays a queue with crossfades. **Autopilot**, which
mixes like a DJ, is **Coming later** (chapter 9).

**Can I edit my mix after recording it?**
Not yet. That is what **Arrange mode** is for (chapter 11). It is **In
development**.

**Can I save my mixes to Google Drive or OneDrive?**
You can set the recording folder (chapter 8) to a folder that your cloud app
syncs, such as a Google Drive, OneDrive or Dropbox folder. An automatic cloud
folder for recordings, tracklists, covers, projects and backups, and "play
from the cloud" with an offline check before a gig, are **Coming later**.
Do not play music straight from a cloud-only folder at a gig: if the internet
drops, the file may not be there.

**Does it work with OBS or an Elgato Stream Deck?**
Not directly yet. Use OBS with BeatFreaks' audio output today. OBS control and
a Stream Deck plugin are **Coming later** (chapter 8).

**Does BeatFreaks send my music or personal information anywhere?**
No. BeatFreaks works offline. It only goes online when you use features that
need it: live broadcasting to your server, MusicBrainz look-ups for tags, or
(later) features you choose to turn on. The planned AI songs with ACE-Step
run on your own PC.

**Where are my cue points and playlists stored?**
In the library database in your settings folder (`mixxxdb.sqlite`). Back up
the whole settings folder (chapter 1) to keep them safe.

**Will BeatFreaks change my music files?**
No, unless you turn on **Synchronize library track metadata from/to file
tags** (it then writes tag edits into the files) or use **Move Track File(s)
to Trash**.

**How do I report a bug?**
See chapter 14.

## 16.2 Glossary

| Word | Meaning |
| --- | --- |
| **ASIO** | A Windows sound driver type with very low delay, used by pro audio devices |
| **Auto DJ** | Plays a queue of tracks with automatic crossfades |
| **Autopilot** | Planned BeatFreaks feature that mixes like a DJ (Coming later) |
| **Automation lane** | A recorded line of one control's moves over time (Arrange mode) |
| **Bar** | A group of beats, usually 4 in dance music |
| **Bass swap** | Cutting the bass of one track while bringing in the other's, on a phrase |
| **Beat grid** | Markers on every beat of a track, used by sync, loops and quantize |
| **Beat jump** | Jumping forward or back by a number of beats |
| **Beatloop** | A loop of a set number of beats |
| **Booth output** | A separate output for the DJ's monitor speaker |
| **BPM** | Beats per minute: the tempo of a track |
| **Buffer (audio buffer)** | A small amount of audio prepared in advance. Bigger = safer but more delay |
| **Camelot wheel** | A key wheel of numbers 1–12 with A (minor) and B (major). Neighbours mix well. BeatFreaks shows it with the **Lancelot** notation |
| **Chain (effect chain)** | Three effects in a row in one effect unit |
| **Clip** | One piece of a track placed on the Arrange timeline |
| **Clipping** | Distortion when the level is too high (meter in the red) |
| **Controller** | Hardware with buttons, knobs and faders that controls the software |
| **Crate** | An unordered box of tracks |
| **Crossfader** | The horizontal fader that blends between left and right decks |
| **CSV** | A simple table file that spreadsheets can open |
| **CUE sheet (.cue)** | A text file listing where each track starts in a long recording |
| **Cue point** | The main starting spot of a track |
| **Deck** | A virtual player that plays one track |
| **DRM** | Copy protection that stops a file from playing in other programs |
| **EQ** | Equaliser: the high, mid and low knobs |
| **Energy score** | BeatFreaks' 0–10 measure of how hard a track hits |
| **Gain** | Input level of a channel |
| **HID** | A type of USB connection used by some controllers (S4 MK3, F1) |
| **Hotcue** | A saved spot in a track you can jump to with one button |
| **Intro / outro markers** | Markers for the start and end sections, used for mixing and Auto DJ |
| **Key** | The musical key of a track |
| **Key lock** | Keeps the key the same when you change tempo |
| **Latency** | Delay between an action and hearing it |
| **Latch / Touch / Read** | Automation recording modes (chapter 11) |
| **Leader (sync leader)** | The deck whose tempo the others follow |
| **Loop roll** | A loop that plays only while held, then continues in time |
| **Mapping** | The file that tells BeatFreaks what each controller button does |
| **Meta knob** | One knob that controls an effect's main setting |
| **MIDI** | A common music-device language; many controllers use it |
| **Mix (knob)** | Dry/wet: how much of the effect you hear |
| **Phrase** | A musical section, usually 8, 16 or 32 bars |
| **PFL** | Pre-fader listen: hearing a deck in your headphones |
| **Playlist** | An ordered list of tracks |
| **Quantize** | Makes cues, loops and jumps snap to the beat |
| **Quick effect** | The per-channel effect knob (normally a filter) |
| **ReplayGain** | Automatic loudness levelling between tracks |
| **Sample rate** | How many times per second sound is measured (for example 44100 Hz) |
| **Sampler** | A small extra deck for short sounds |
| **SHIFT** | A controller button that gives other buttons a second job while held |
| **Skin** | The look and layout of the window |
| **Slip mode** | Loops and scratches do not move the track; it continues in time afterwards |
| **Smart Next** | BeatFreaks feature that ranks the next track (In development) |
| **Soft takeover** | A knob or fader does nothing until it reaches the current value, to avoid jumps |
| **Song Inbox** | BeatFreaks feature that imports new downloads automatically (In development) |
| **Stems** | Separate parts of a track (drums, bass, melody, vocals) |
| **Super knob** | One knob that moves several effect settings at once |
| **Sync** | Matches tempo (and with quantize, beat position) between decks |
| **Tempo slider** | Changes a deck's speed (also called pitch fader) |
| **WASAPI** | A standard Windows sound driver type |
| **Waveform** | The picture of the track's sound |

Next: [Credits and licence](17-credits-and-licence.md)
