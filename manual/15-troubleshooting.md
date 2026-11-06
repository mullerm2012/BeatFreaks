# 15. Troubleshooting

Find your problem below and work through the steps in order. If nothing helps,
send your log (chapter 14).

Quick index:

1. [No sound](#151-no-sound)
2. [No sound in the headphones](#152-no-sound-in-the-headphones)
3. [Crackles, clicks or drop-outs](#153-crackles-clicks-or-drop-outs)
4. [Controller not detected](#154-controller-not-detected)
5. [Controller connected but buttons do the wrong thing](#155-controller-connected-but-buttons-do-the-wrong-thing)
6. [Wrong BPM or beat grid](#156-wrong-bpm-or-beat-grid)
7. [Wrong or missing key](#157-wrong-or-missing-key)
8. [Tracks missing from the library](#158-tracks-missing-from-the-library)
9. [A track will not load or play](#159-a-track-will-not-load-or-play)
10. [Sync does not line up the beats](#1510-sync-does-not-line-up-the-beats)
11. [Auto DJ will not start](#1511-auto-dj-will-not-start)
12. [Recording is silent or distorted](#1512-recording-is-silent-or-distorted)
13. [Broadcast shows ERROR](#1513-broadcast-shows-error)
14. [BeatFreaks crashes or shows a black window at start](#1514-beatfreaks-crashes-or-shows-a-black-window-at-start)
15. [Waveforms are slow or the screen stutters](#1515-waveforms-are-slow-or-the-screen-stutters)
16. [Windows will not let me install](#1516-windows-will-not-let-me-install)
17. [My Mixxx settings changed after installing BeatFreaks](#1517-my-mixxx-settings-changed-after-installing-beatfreaks)
18. [I cannot find Smart Next, Arrange, AI songs or the tracklist export](#1518-i-cannot-find-smart-next-arrange-ai-songs-or-the-tracklist-export)

## 15.1 No sound

1. Is a deck playing? The play button should be lit and the waveform moving.
2. Is the channel **volume fader** up, and the **crossfader** on that deck's
   side (or in the middle)?
3. Is the channel **gain** turned down, or an **EQ kill** on? Double-click the
   knobs to reset them.
4. Is the **main** volume up (on screen, and on your controller or speakers)?
5. Open **Preferences → Sound Hardware**:
   - Is **Main** set to a device under the **Output** tab?
   - Is it the device your speakers are plugged into?
   - Click **Query Devices** and choose the device again.
6. On the S4 MK3: are the speakers connected to the S4's main outputs and is
   the S4's own **MAIN** knob turned up? (That knob works inside the S4.)
7. Another program (a browser, a video call) may be holding the sound card,
   especially with ASIO or WASAPI. Close other audio programs and restart
   BeatFreaks.
8. Try another **Sound API** (for example WASAPI instead of ASIO).
9. Check Windows: click the speaker icon in the taskbar and make sure the
   device is not muted.

## 15.2 No sound in the headphones

1. Is the deck's **headphone (PFL/CUE)** button lit?
2. Is **Headphones** set in **Preferences → Sound Hardware → Output**? It needs
   its own output channels (for the S4 MK3: channels 3–4).
3. Is the **headphone volume** up? On the S4 MK3 use the controller's headphone
   **VOL** knob.
4. Is **headphone mix** (cue mix) fully on the main side? Turn it toward cue.
5. Laptop only, no extra sound card: most laptops have only one output, so
   headphone cueing is not possible without an extra USB audio interface or
   a controller with a sound card.

## 15.3 Crackles, clicks or drop-outs

These happen when the PC cannot fill the sound buffer in time.

1. Open **Preferences → Sound Hardware** and **increase the Audio Buffer** one
   step. Click **Apply** and test. Repeat until clean.
2. Watch **Buffer Underflow Count** on that page and the **Buffer %** meter in
   the toolbar. If they rise, the buffer is still too small or the PC is too
   busy.
3. Use **ASIO** if your device has an ASIO driver; otherwise **WASAPI**.
4. Laptop: plug in the power adapter, and in Windows choose the **Best
   performance** power mode (Settings → System → Power).
5. Close other programs, especially browsers, video calls and cloud sync
   (OneDrive, Google Drive, Dropbox) during a set.
6. Use a faster **Keylock/Pitch-Bending Engine** (chapter 2), or turn key lock
   off on decks that do not need it.
7. Turn off effects you are not using. Hide waveforms or lower **Preferences →
   Waveforms** frame rate.
8. Plug the controller directly into the PC, not through a hub. Try another
   USB port.
9. Turn off Wi-Fi and Bluetooth during a gig if you do not need them (some
   drivers cause short stalls).
10. Make sure the controller's sound card driver is up to date (from the maker).

## 15.4 Controller not detected

1. Is it switched on and plugged in? For the S4 MK3, is the power adapter
   connected?
2. Did you plug it in **before** starting BeatFreaks? If not, close and
   restart BeatFreaks.
3. Is the maker's driver installed (Native Instruments driver for the S4 MK3
   and F1)?
4. Try another USB cable and another USB port (directly on the PC).
5. Close other DJ programs (Traktor, Serato, rekordbox, Mixxx). Only one
   program can use an HID controller at a time.
6. Open **Preferences → Controllers**. Is the device listed on the left?
   - **Listed:** click it, choose the mapping in **Load Mapping**, tick
     **Enabled**, click **Apply**.
   - **Not listed:** Windows does not see it either. Check in Windows
     **Device Manager** (right-click Start → Device Manager) for a warning
     sign on the device.
7. Still not working? Start BeatFreaks with `--controller-debug` (chapter 14,
   section 14.3) and send the log.

## 15.5 Controller connected but buttons do the wrong thing

1. Check the mapping in **Preferences → Controllers → Load Mapping**. It must
   match your exact model (for example S4 **MK3**, not MK2).
2. Read your controller's section in chapter 7. Some buttons need **SHIFT**,
   and some work only while held.
3. On the S4 MK3, check which deck each side controls (deck buttons A/C and
   B/D).
4. Check the **Mapping Settings** for that controller. Options there change
   button jobs (for example key lock on SHIFT + MASTER).
5. Two controllers might be controlling the same thing (for example the F1's
   faders in decks mode and the S4's faders). See chapter 7, section 7.6.
6. The S4 MK3 **TT** button does nothing unless motors are allowed in the
   mapping settings. That is normal.
7. The S4 MK3 **MUTE** pad mode and some F1 buttons have no job in their
   mappings. That is normal.
8. Report it with a `--controller-debug` log (chapter 14).

## 15.6 Wrong BPM or beat grid

1. **Half or double** (70 instead of 140, or the other way): right-click the
   track → **Adjust BPM** → **Double BPM** or **Halve BPM**.
2. **Slightly off**: right-click → **Analyze** → **Reanalyze (constant BPM)**.
   For live-played music with a drifting tempo try **Reanalyze (variable
   BPM)**.
3. **Grid starts in the wrong place**: play the track, and when a strong beat
   is under the play marker, click the deck's **Adjust Beatgrid** button. On
   the S4 MK3 press **GRID**.
4. **Tap it**: right-click → **Properties** → **BPM** tab → **Tap to Beat**.
5. **Lock** the corrected BPM: right-click → **Adjust BPM** → **Lock BPM**.
6. For a whole library of electronic music, make sure **Preferences → Beat
   Detection → Assume constant tempo** is on.

## 15.7 Wrong or missing key

1. Make sure **Preferences → Key Detection → Enable Key Detection** is on.
2. Re-analyse the track: right-click → **Analyze** → **Reanalyze**.
3. If you prefer Camelot-style numbers, set **Key Notation** to **Lancelot**.
4. Key detection is never perfect. Trust your ears; change a wrong key in
   **Properties**.

## 15.8 Tracks missing from the library

1. Is the folder in **Preferences → Library → Music Directories**? Add it.
2. Click **Library → Rescan Library**.
3. Is the file type supported? See **Preferences → Library → Audio File
   Formats**.
4. Did you move the music to a new drive or folder? Use **Relink** in
   **Music Directories**.
5. Is a search filter active? Clear the search box (**Esc**).
6. Did you hide the track? Click the arrow next to **Tracks** in the sidebar
   and open **Hidden Tracks**; select it and click **Unhide**. Files that
   BeatFreaks can no longer find are listed under **Missing Tracks**.

## 15.9 A track will not load or play

1. Is the file still on disk, at the same place? (Unplugged USB drive?)
2. Is the deck already playing? By default you cannot load onto a playing
   deck (**Preferences → Decks → Loading a track, when deck is playing**).
3. Some files are copy-protected (DRM, for example from old music stores or
   streaming apps). They cannot be played.
4. Try the file in another player. If it fails there too, the file is damaged.

## 15.10 Sync does not line up the beats

1. **Hold** SYNC for a second to turn on sync lock. A quick tap only matches
   the tempo once.
2. Turn on **Quantize** on both decks so beats are kept in phase.
3. Check both tracks' beat grids (15.6). Sync can only be as good as the grids.
4. Check which deck is the **leader** (crown button).

## 15.11 Auto DJ will not start

Read the message BeatFreaks shows. Usually:

- Decks 1 and 2 must be on **opposite sides of the crossfader**.
- At least one of them must be **stopped**.
- Decks 3 and 4 (if not used) must be stopped.
- The queue must not be empty.

## 15.12 Recording is silent or distorted

1. The recording takes the **main mix**. If the main meter is quiet or red,
   so is the recording.
2. Check that **Main Mix** is **Enabled** in Sound Hardware.
3. Check the recording folder exists and you have space on the drive.
4. Distortion: lower channel gains or the main level so the main meter stays
   out of the red.

## 15.13 Broadcast shows ERROR

1. Check your internet connection.
2. In **Preferences → Live Broadcasting**, check **Type**, **Host**, **Port**,
   **Mount**, **Login** and **Password** against what your streaming host gave
   you.
3. Check the **Format** (MP3, Ogg Vorbis …) is one your host accepts.
4. Firewalls or company networks can block streaming ports.
5. Turn on **Enable automatic reconnect** for short drops.

## 15.14 BeatFreaks crashes or shows a black window at start

1. Start it once with `--safe-mode` (chapter 14, section 14.3).
2. Update your graphics driver from your graphics card maker's website.
3. Unplug controllers and start again. If it works, plug them in one at a time
   to find the cause.
4. Send `mixxx.log` from before the next start (chapter 14).

## 15.15 Waveforms are slow or the screen stutters

1. **Preferences → Waveforms**: choose a simpler waveform type and a lower
   frame rate.
2. Update your graphics driver.
3. Hide waveforms you do not need (**WAVEFORMS** in the toolbar).
4. Lower **Preferences → Interface → HiDPI / Retina scaling** if it is very
   high.

## 15.16 Windows will not let me install

1. "Windows protected your PC": click **More info → Run anyway** (chapter 1,
   section 1.4), only for installers from the BeatFreaks team.
2. "This app has been blocked by your system administrator": the PC is managed
   by a company or school. Ask the administrator, or use another PC.
3. Not enough rights: you need an administrator account to install.
4. Windows Sandbox missing: it needs Windows Pro, Enterprise or Education
   (chapter 1, section 1.5).

## 15.17 My Mixxx settings changed after installing BeatFreaks

Very early BeatFreaks test installers still used the Mixxx name. Windows
treated them as a Mixxx update, so they replaced the Mixxx program and shared
its settings folder (`%LOCALAPPDATA%\Mixxx`).

1. Uninstall the early test build (chapter 1, section 1.7).
2. Reinstall Mixxx from the Mixxx website if you use it.
3. Install a BeatFreaks-named installer. It keeps its own settings in
   `%LOCALAPPDATA%\BeatFreaks`.

## 15.18 I cannot find Smart Next, Arrange, AI songs or the tracklist export

These are **In development**: their engines exist, but there are no buttons or
screens in the program yet. See chapters 10–13 for what they will do and what
you can use today instead.

Next: [FAQ and glossary](16-faq-and-glossary.md)
