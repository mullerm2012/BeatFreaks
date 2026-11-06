# BeatFreaks User Manual

Welcome to BeatFreaks, a DJ program for Windows.

BeatFreaks is built on **Mixxx**, the free and open-source DJ software. It
keeps everything Mixxx does (four decks, sync, key lock, loops, hotcues,
effects, recording, Auto DJ and more than a hundred controller mappings) and
adds new tools for DJs, such as Smart Next and an Arrange timeline.

This manual is written for DJs, not for programmers. Each chapter uses short
steps and explains every new word the first time it appears.

![Screenshot: BeatFreaks main window with two tracks loaded and playing](images/main-window-overview.png)

## Read this first: what works today

BeatFreaks is young. Some features are finished, some are being built, and
some are only planned. This manual never pretends that a feature works when it
does not. Every feature is marked with one of these labels:

| Label | What it means for you |
| --- | --- |
| **Available now** | You can use it in the current version. |
| **In development** | The engine behind it is written and tested, but there is no button or screen for it yet. You cannot use it from the program today. |
| **Coming later** | Planned. Nothing you can use yet. |

Quick overview:

| Feature | Status | Chapter |
| --- | --- | --- |
| Decks, mixer, sync, key lock, EQ, crossfader, headphone cue | Available now | [5](05-decks-and-mixing.md) |
| Library, search, crates, playlists, history, BPM and key analysis | Available now | [4](04-library.md) |
| Loops, hotcues, beat jump, effects, samplers | Available now | [6](06-loops-cues-effects.md) |
| Controllers (164 built-in mappings, including Traktor Kontrol S4 MK3 and F1) | Available now (BeatFreaks testing on the S4 MK3 and F1 is still going on) | [7](07-controllers.md) |
| Recording a mix and live broadcasting | Available now | [8](08-recording-and-streaming.md) |
| Auto DJ | Available now | [9](09-auto-dj.md) |
| Energy score (how hard a track hits) | In development (it is already measured in the background, but not shown) | [10](10-smart-next.md) |
| Smart Next (suggests the next track) | In development | [10](10-smart-next.md) |
| Arrange mode and automation lanes | In development | [11](11-arrange-mode.md) |
| AI songs (ACE-Step, Send to Suno, Song Inbox) | In development | [12](12-ai-songs.md) |
| Tracklist, YouTube chapters, CUE and CSV export, mix cover | In development | [13](13-tracklist-and-mix-cover.md) |
| Hardware Test screen and one-click "Send log" | In development | [14](14-sending-logs-and-feedback.md) |
| Autopilot (mixes on its own) | Coming later | [10](10-smart-next.md), [16](16-faq-and-glossary.md) |
| Phone remote, cloud folder, OBS and Stream Deck, streaming services | Coming later | [16](16-faq-and-glossary.md) |
| Pioneer DDJ-WeGO4 and Numark 4TRAK support | Coming later | [7](07-controllers.md) |

## Table of contents

1. [Installing BeatFreaks](01-install.md): what your PC needs, downloading,
   the installer, the Windows "protected your PC" warning, testing safely in
   Windows Sandbox, updating and uninstalling.
2. [First run](02-first-run.md): picking your music folder, the first library
   scan, setting up your sound card or controller audio, and latency.
3. [The screen](03-the-screen.md): every area of the main window and every
   menu.
4. [The library](04-library.md): search, crates, playlists, history, and BPM
   and key analysis.
5. [Decks and mixing](05-decks-and-mixing.md): loading, play, cue, sync,
   pitch, key lock, EQ, crossfader and headphone cue.
6. [Loops, cues and effects](06-loops-cues-effects.md): hotcues, intro and
   outro markers, loops, beat jump, effects and samplers.
7. [Controllers](07-controllers.md): connecting a controller, choosing a
   mapping, the full Traktor Kontrol S4 MK3 layout with SHIFT functions, the
   Kontrol F1, using several controllers, and MIDI learning.
8. [Recording and streaming](08-recording-and-streaming.md): recording a mix
   to a file and broadcasting live.
9. [Auto DJ](09-auto-dj.md): let BeatFreaks play a list of tracks with
   automatic crossfades.
10. [Smart Next](10-smart-next.md): how BeatFreaks will pick the next track by
    key, tempo, energy and crowd reaction.
11. [Arrange mode](11-arrange-mode.md): the timeline that records your live
    moves so you can fix a mix afterwards.
12. [AI songs](12-ai-songs.md): making new tracks with ACE-Step on your own PC,
    the Send to Suno route, the Song Inbox, and the rights you need.
13. [Tracklist and mix cover](13-tracklist-and-mix-cover.md): what you played,
    ready to post, plus a cover image for your mix.
14. [Sending logs and feedback](14-sending-logs-and-feedback.md): where the log
    file is and how to report a problem.
15. [Troubleshooting](15-troubleshooting.md): no sound, crackles, controller
    not found, wrong BPM and more.
16. [FAQ and glossary](16-faq-and-glossary.md): common questions and every DJ
    word used in this manual.
17. [Credits and licence](17-credits-and-licence.md): Mixxx, the GNU GPL,
    getting the source code, and trademarks.

## How to read the steps

- **Bold words** are the names of buttons, menus or settings exactly as they
  appear on screen.
- "Click **Options → Preferences**" means: click the **Options** menu, then
  click **Preferences** in the list that opens.
- On a controller, button names are written in capitals, for example
  **SHIFT** or **PLAY**.
- "SHIFT + HOTCUE" means: hold **SHIFT** down, press **HOTCUE**, then let go
  of both.

## A note about the name

BeatFreaks is built from Mixxx. Some windows, settings and file names still say
"Mixxx" (for example the log file is called `mixxx.log`). That is normal. It is
the same program.

![Screenshot: Help menu open showing About, Settings directory and Keyboard Shortcuts](images/help-menu.png)
