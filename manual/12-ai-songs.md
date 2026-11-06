# 12. AI songs

> **Status: In development.** The parts below are written and tested as
> engines, but there are **no AI song buttons or screens in BeatFreaks yet**.
> You cannot make AI songs from inside the program today. This chapter
> explains how it will work, and what you can already prepare.

| Part | Status |
| --- | --- |
| **Song brief** (describes the song to make, from what is happening on the floor) | In development |
| **ACE-Step client** (talks to the free ACE-Step 1.5 program running on your own PC) | In development (tested with a simulated server, not yet with a real ACE-Step install) |
| **Send to Suno** (ready-to-paste text for the Suno website) | In development |
| **Song Inbox** (watches your Downloads folder and imports new songs) | In development |
| AI songs screen, buttons, progress display | Coming later |
| Official Suno connection inside BeatFreaks | Coming later (only if BeatFreaks is accepted into Suno's partner programme) |
| Loops and stems at project tempo and key (Studio) | Coming later |

## 12.1 The idea

Sometimes the floor needs something you do not have: a warm-up groove in the
right key, a peak-time track at exactly your tempo, or a DJ tool with drums and
bass for a long blend. BeatFreaks will help you make one:

1. BeatFreaks looks at what is playing: **tempo**, **key**, **energy** and
   what has worked tonight.
2. You choose a **purpose** (see 12.2).
3. BeatFreaks writes a **song brief**: genre, mood, tempo, key, energy,
   vocals, length, and a DJ-friendly structure.
4. You choose a route:
   - **ACE-Step** (free, runs on your own PC, no internet needed), or
   - **Send to Suno** (copy the text into the Suno website yourself).
5. The finished song lands in your library through the **Song Inbox**.
6. You **listen and approve** it before you play it to anyone (see 12.7).

## 12.2 Song purposes

| Purpose | What the brief asks for |
| --- | --- |
| **Warm-up** | Deep, rolling, hypnotic groove. Energy at most 5. No vocals |
| **Fill the floor** | Irresistible groove with a catchy hook. Energy 1 point above what is playing (2 points if the floor is cold) |
| **Peak banger** | Euphoric build-up and a huge drop. Energy at least 8.5. Key moved one step up the Camelot wheel (an "energy boost") |
| **Cool down** | Melodic, warm, chilled. Energy 2 points lower. Relative key (same number, other letter) |
| **DJ tool** | Minimal drums and bass, loop-friendly, no vocals, shorter (96 bars) |

In every case the tempo is the playing track's tempo, rounded to a whole BPM,
so the new song mixes straight in.

## 12.3 What is in a song brief

| Setting | Default | Notes |
| --- | --- | --- |
| Genre | house (or the genre that worked best tonight) | |
| Mood | from the purpose | |
| Extra style | empty | Your own words, added to the style |
| BPM | 124 (or the playing track) | 30–300 |
| Key | the playing track's key (or "let the model choose") | |
| Energy | 6 | 0–10 |
| Vocals | Hook | **None** (instrumental), **Hook** (a short vocal hook), **Full** (verses and chorus) |
| Theme | from the purpose | What the hook is about |
| Lyrics | empty | Your own lyrics. Empty means the model writes them |
| Language | English | |
| DJ-friendly | on | Long beat-only intro and outro so you can mix it |
| Intro / outro | 32 bars each | Kept on 8-bar phrase boundaries |
| Length | 128 bars | Limited to 10 seconds – 10 minutes |
| Versions | 2 | 1–8 versions per request |
| Clean lyrics | on | No explicit words (club and radio safe) |

The lyrics are laid out with section tags such as **[Intro]**, **[Build-Up]**
and **[Drop]**, so the generator builds a DJ-friendly structure.

## 12.4 Route 1: ACE-Step on your own PC

**ACE-Step 1.5** is a free music generator (MIT licence for its code) that
runs on your own computer. Nothing is sent to the internet. BeatFreaks will
talk to it over a local connection.

### What you need

- A Windows PC with plenty of memory.
- A graphics card makes it much faster. One with about 4 GB of its own memory
  is a sensible minimum. Without a graphics card ACE-Step can run on the
  processor, but it is slow: one song may take many minutes. (CPU-only speed
  has not been measured by the BeatFreaks team yet.)
- Disk space for the AI model (several gigabytes).

### Preparing ACE-Step (you can do this now)

ACE-Step is a separate program from a separate project. Follow the
installation steps in the ACE-Step 1.5 project's own documentation (its
GitHub page, `ace-step/ACE-Step-1.5`). In short, the project is started with
its API server using the command:

```
uv run acestep-api
```

When it runs, it listens at `http://127.0.0.1:8001` on your PC. That is the
address BeatFreaks will use by default. (`127.0.0.1` means "this computer".)

If you start ACE-Step with an **API key** (a password for its server), you
will enter the same key in BeatFreaks.

### How BeatFreaks will use it (when the screen is ready)

1. BeatFreaks checks that ACE-Step is running (a health check).
2. It sends the brief. ACE-Step puts it in its queue.
3. BeatFreaks shows a timer while it works and checks for progress every few
   seconds.
4. When the song versions are ready, BeatFreaks downloads them, one at a time,
   into your AI songs folder.
5. Next to each song it saves a small **recipe** file ending in `.bfai.json`
   with the prompt, lyrics, seed and model used, so you can recreate or tweak
   it later.

Safety and reliability built into the engine:

- If ACE-Step is busy or not started yet, BeatFreaks waits and tries again a
  few times.
- A job is stopped after 30 minutes by default (slow PCs).
- You can cancel a job.
- Several jobs can run at once.
- BeatFreaks only downloads from the exact ACE-Step address you set, never
  from anywhere else, so your API key is never sent to another server.
- Each download has a size limit and is checked to be a real audio file
  before it is saved.
- File names are cleaned so they are valid on Windows.

![Screenshot: (planned) AI songs panel with purpose buttons, the brief, Generate with ACE-Step and Send to Suno](images/ai-songs-panel-planned.png)

## 12.5 Route 2: Send to Suno

**Suno** is an online AI music service with its own website and account.
BeatFreaks does not log in to Suno for you and does not use any unofficial
Suno connection.

How it will work:

1. In BeatFreaks, click **Send to Suno** (when the button exists).
2. BeatFreaks copies a ready-made text: first a **style line** (genre, mood,
   tempo, key, energy), then the **lyrics** with section tags.
3. Open the Suno website in your browser, sign in, and paste the text into
   its custom song fields.
4. Make the song on Suno and **download** it.
5. The **Song Inbox** picks it up from your Downloads folder.

Your Suno plan decides what you may do with the songs (for example, whether
you may play them in public or use them commercially). Read Suno's terms for
your plan.

## 12.6 The Song Inbox

The Song Inbox watches chosen folders, usually your **Downloads** folder and
the AI songs folder, and adds each new song to your library as soon as it is
completely downloaded.

How it behaves:

- Files that are **already** in the folder when the inbox starts are left
  alone. It never imports your whole Downloads folder.
- It checks the folders about every 1.5 seconds.
- A file counts as finished when its size and date have not changed for two
  checks in a row.
- Browser part-files are ignored (names ending in `.crdownload`, `.part`,
  `.partial`, `.tmp`, `.download`, `.opdownload`).
- Tiny files (under 64 KB) are ignored.
- Audio types it accepts: MP3, WAV, FLAC, AIF/AIFF, M4A, AAC, OGG, Opus and
  WavPack.
- Songs with a `.bfai.json` recipe next to them are labelled **BeatFreaks AI**.
  Other new songs (for example a Suno download, a Beatport or Bandcamp
  purchase, or a promo) are labelled **Inbox**.

Today, without the inbox, put downloaded songs in one of your music folders
and use **Library → Rescan Library** (chapter 4).

## 12.7 Rights and approval: read this before you play AI songs

- **Listen first.** Always listen to an AI song all the way through and
  approve it yourself before you play it in public. AI can produce odd
  sections, mistakes in lyrics, or sounds you do not want.
- **AI songs are labelled.** BeatFreaks keeps a recipe file next to each song
  it made, and the tracklist tools count and mark AI-made songs
  (chapter 13). Be open with your audience and venue.
- **Check the terms.** ACE-Step's code is under the MIT licence; check the
  licence of the model files you download too. Suno's terms depend on your
  plan.
- **Do not imitate real artists.** Do not ask for a song "in the voice of" a
  real singer or copy someone's lyrics. Write your own words or let the model
  write them.
- **Similarity.** AI output can sound close to existing music. If a result
  sounds like a known song, do not use it.
- **Clean lyrics** are on by default for club and radio safety.

This is general information, not legal advice.

Next: [Tracklist and mix cover](13-tracklist-and-mix-cover.md)
