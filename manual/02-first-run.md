# 2. First run

> **Status: Available now**

This chapter walks you through the first start: choosing your music folder,
the first library scan, setting up sound, and getting low latency.

Before you start:

- If you have a DJ controller, plug it in with its USB cable and switch it on
  **before** you start BeatFreaks.
- For a Traktor Kontrol S4 MK3 or F1, install Native Instruments' own driver
  for that device first (from Native Instruments' website, using their
  installer). The S4 MK3 needs its driver for its built-in sound card.

## 2.1 Start BeatFreaks

1. Click **Start** and click **BeatFreaks**.
2. A splash screen with a progress bar appears while the program loads.
3. The first time, a folder window called **Choose music library directory**
   opens. This is where you tell BeatFreaks where your music is.

![Screenshot: splash screen while BeatFreaks starts](images/splash.png)

## 2.2 Choose your music folder

1. In the **Choose music library directory** window, go to the folder that
   holds your music. Windows starts you in your **Music** folder
   (`C:\Users\<your name>\Music`).
2. Click the folder once to select it. Pick the top folder: BeatFreaks also
   looks inside every folder below it.
3. Click **Select Folder**.

![Screenshot: Choose music library directory window with the Music folder selected](images/choose-music-folder.png)

If you click **Cancel**, BeatFreaks still opens with an empty library. You can
add folders at any time (see below).

### Adding more music folders later

Do this if your music is in several places, for example a second drive.

1. Click **Options → Preferences**.
2. Click **Library** on the left.
3. Under **Music Directories**, click **Add**.
4. Pick the folder and click **Select Folder**.
5. Click **OK**. BeatFreaks scans the new folder.

Other buttons in that box:

- **Remove**: stop watching a folder. Your files are not deleted.
- **Relink**: use this if you moved a music folder to a new place (for
  example a new drive letter). Point it at the new place and your cue points,
  loops and playlists stay connected to the files.
- **Rescan directories on start-up**: tick this if you add music often. It
  makes start-up a little slower.

![Screenshot: Preferences, Library page with the Music Directories box](images/prefs-library-music-dirs.png)

## 2.3 The first library scan

After you pick a folder, BeatFreaks reads every music file in it and adds it to
the library. This is called a **scan**.

- A scan reads tags (artist, title, album, genre and so on). It does not
  change your files.
- Large collections can take several minutes. You can keep using the program
  while it runs.
- To scan again later, click **Library → Rescan Library**.

Supported file types include MP3, WAV, AIFF, FLAC, Ogg Vorbis, Opus, M4A/AAC
and WavPack. The full list is shown under **Options → Preferences → Library →
Audio File Formats**.

### BPM and key analysis

The scan does not yet know each track's tempo (BPM, beats per minute) or
musical key. These are worked out by **analysis**:

- Each track is analysed automatically the first time you load it into a deck.
- To analyse many tracks in advance, use the **Analyze** view (chapter 4,
  section 4.8). Do this before a gig so loading is instant.

While BeatFreaks analyses tracks, it also measures an **energy score** for
each one (how hard it hits, from 0 to 10). This runs in the background
already, but you cannot see it yet. It will be used by Smart Next (chapter
10). Status: **In development**.

## 2.4 Set up your sound (Sound Hardware)

BeatFreaks needs to know where to send the music (your speakers) and the
preview (your headphones).

1. Click **Options → Preferences**.
2. Click **Sound Hardware** on the left.

![Screenshot: Preferences, Sound Hardware page with Sound API, Sample Rate and Audio Buffer](images/prefs-sound-hardware.png)

### Step 1: pick the Sound API

The **Sound API** is the way BeatFreaks talks to your sound card. On Windows
you may see:

| Sound API | When to use it |
| --- | --- |
| **ASIO** | Best choice if your controller or audio interface has an ASIO driver (the S4 MK3 driver from Native Instruments includes one). Lowest delay. |
| **Windows WASAPI** | Good choice for built-in sound and devices without ASIO. |
| **Windows DirectSound** / **MME** | Older methods. Use only if nothing else works. They have more delay. |

Only the APIs that your PC and drivers support are listed. If you do not see
**ASIO**, your device has no ASIO driver installed, or this build does not
include ASIO support. Use **Windows WASAPI** instead.

### Step 2: choose outputs

At the bottom of the page there are two tabs, **Output** and **Input**.

1. Click the **Output** tab.
2. Next to **Main**, choose the device and channels for your speakers.
3. Next to **Headphones**, choose the device and channels for your
   headphones.
4. Leave **Booth** empty unless you have a separate booth monitor output.

Typical setups:

| Your hardware | Main | Headphones |
| --- | --- | --- |
| Traktor Kontrol S4 MK3 | S4 MK3, channels **1–2** | S4 MK3, channels **3–4** |
| USB audio interface with 4 outputs | Interface, channels 1–2 | Interface, channels 3–4 |
| Laptop only, no extra hardware | Built-in speakers or headphone socket | Leave empty (you cannot pre-listen) |

The S4 MK3's own **MAIN**, **BOOTH**, headphone **VOL** and **CUE MIX** knobs
work inside the controller's sound card, not inside BeatFreaks, when you use
the S4's outputs. That is the normal setting. Chapter 7 explains the options.

![Screenshot: Sound Hardware Output tab with Main on channels 1-2 and Headphones on channels 3-4 of the S4 MK3](images/prefs-sound-outputs-s4.png)

### Step 3: sample rate

**Sample Rate** is how many times per second the sound is measured. **44100
Hz** or **48000 Hz** are both fine. If your device has a fixed rate, pick that
one.

### Step 4: audio buffer (latency)

**Latency** is the delay between pressing a button and hearing the result. The
**Audio Buffer** setting controls it.

- A **smaller** buffer means less delay and a tighter feel when scratching or
  using cue buttons.
- A **larger** buffer means fewer clicks and crackles, but more delay.

Good starting points:

| Sound API | Start with |
| --- | --- |
| ASIO | about 5–10 ms |
| WASAPI | about 10–20 ms |
| DirectSound / MME | 40 ms or more |

How to find the best setting:

1. Pick a buffer from the list.
2. Click **Apply**.
3. Play a track for a few minutes, with effects on.
4. Watch **Buffer Underflow Count** under **Hints and Diagnostics**. If the
   number goes up, or you hear clicks, choose a larger buffer.
5. If everything is clean, you can try a smaller one.

The **System Reported Latency** line shows the real delay.

### Other settings on this page

- **Keylock/Pitch-Bending Engine**: the method used to keep the key steady when
  you change tempo (see key lock in chapter 5). Each choice says how fast it
  is and how good it sounds, for example **Soundtouch (fastest, low
  quality)** or **Rubberband (fast, medium quality)**. The "R3" Rubberband
  choices sound best but need a stronger processor. On older laptops pick a
  faster one if you hear crackles with key lock on.
- **Engine Clock**: use **Soundcard Clock** when you play to a crowd. Use
  **Network Clock** only for broadcasting with no live audience.
- **Main Mix**: leave **Enabled** unless you use an external hardware mixer.
- **Microphone Monitor Mode** and **Microphone Latency Compensation**: only
  matter if you use a microphone input (see section 2.5).
- **Main / Headphone / Booth Output Delay**: lets you delay one output to line
  it up with another. Leave at 0 unless you know you need it.
- **Query Devices**: click this after plugging in a new sound device so it
  appears in the lists.

When you are done, click **OK**.

## 2.5 Inputs (microphone, line in, vinyl control)

Use the **Input** tab only if you want to:

- Talk over the music with a microphone (set **Microphone 1**).
- Play an external source, such as a phone or turntable, through BeatFreaks
  (set **Auxiliary 1**).
- Use timecode vinyl (set **Vinyl Control 1** and see **Options → Vinyl
  Control**).

To show the microphone controls on screen, click **View → Show Microphone
Section**.

## 2.6 Check that everything works

1. Click a track in the library.
2. Drag it onto the left deck (or press **Shift + Left Arrow** with keyboard
   shortcuts on).
3. Click the deck's **Play** button.
4. Push the left channel's volume fader up and the crossfader to the left or
   the middle.
5. You should hear music from your speakers.
6. Click the deck's headphone button (**PFL**). You should hear it in your
   headphones.

No sound? See chapter 15, section 15.1.

## 2.7 Turn on keyboard shortcuts (optional)

You can DJ with the computer keyboard.

1. Click **Options → Enable Keyboard Shortcuts** so it has a tick.
2. Click **Help → Keyboard Shortcuts** to see the full list.

The most useful ones (US keyboard layout):

| Action | Left deck (1) | Right deck (2) |
| --- | --- | --- |
| Play / pause | **D** | **L** |
| Cue | **F** | **;** |
| Sync | **1** | **6** |
| Headphone (PFL) | **T** | **Y** |
| Hotcues 1–4 | **Z X C V** | **M , . /** |
| Delete hotcues 1–4 | **Shift + Z X C V** | **Shift + M , . /** |
| Beat loop | **Q** | **U** |
| Halve / double loop | **W** / **E** | **I** / **O** |
| Loop in / out | **2** / **3** | **7** / **8** |
| Loop on/off (reloop) | **4** | **9** |
| Beat jump back / forward | **A** / **S** | **J** / **K** |
| Load selected track | **Shift + Left** | **Shift + Right** |
| Tempo down / up | **F1** / **F2** | **F5** / **F6** |

Crossfader: **G** (left) and **H** (right). Search the library: **Ctrl + F**.
Full screen: **F11**. Preferences: **Ctrl + P**.

Next: [The screen](03-the-screen.md)
