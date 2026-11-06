# 13. Tracklist and mix cover

> **Status: In development.** The engine that turns "what you played" into a
> posting-ready tracklist, YouTube chapters, a CUE sheet, CSV and JSON files,
> and that plans a mix cover layout, is written and tested. There is **no
> button or screen for it yet**, and the cover picture itself is not drawn
> yet. You cannot use these exports from the program today.
>
> **Available now** instead: the **History** list (chapter 4), **Export
> Playlist** from a history list, and Mixxx's **Create a CUE file** option
> when recording (chapter 8).

## 13.1 What you will get

After a set or a recorded mix, BeatFreaks will make everything you usually
type up by hand before posting:

| Output | What it looks like | Where to use it |
| --- | --- | --- |
| **Tracklist text** | `01. [00:00] Artist – Title (8A, 124 BPM)` | SoundCloud, Mixcloud, Instagram, Facebook |
| **YouTube chapters** | `00:00 Artist – Title` lines | Paste into a YouTube video description to get clickable chapters |
| **CUE sheet** | A `.cue` file | Lets music players split the recorded mix into separate tracks |
| **CSV** | `#,Start,Artist,Title,Key,BPM,AI-made` | Excel, Google Sheets, your own records |
| **JSON** | Structured text | For a website or a stream overlay |
| **Set stats** | Length, number of tracks, BPM range, key journey, number of AI-made songs | Your own notes, social posts |
| **Mix cover plan** | Where each picture and the title go on a square cover | A cover image for the mix |

![Screenshot: (planned) Tracklist panel with the text tracklist, a Copy button, and tabs for YouTube, CUE, CSV and Cover](images/tracklist-panel-planned.png)

## 13.2 How the list is cleaned

The raw history includes every track that was ever live. The tracklist tools
tidy it:

- Tracks are put in time order.
- A track that was on air for **less than 30 seconds** is left out (a quick
  preview or a mistake). The last track is always kept.
- If the same song is loaded again straight after itself (a restart), it
  appears once.
- Times are counted from the start of the mix. Sets longer than an hour use
  `h:mm:ss`.

## 13.3 The text tracklist

Example:

```
Friday Night Warm-up
DJ Name · 2026-11-06

01. [00:00] Artist A – First Track (8A, 122 BPM)
02. [04:12] Artist B – Second Track (9A, 123 BPM)
03. [08:47] Artist C – Third Track (9B, 124 BPM, AI-made)

Mixed live in BeatFreaks
```

- The title, DJ name and date lines appear if you fill them in.
- Key and BPM appear if the tracks were analysed.
- **AI-made** marks songs made with BeatFreaks AI, ACE-Step or Suno
  (chapter 12).
- The last line credits BeatFreaks; this can be switched off.
- Tracks with no title show as "Unknown".

## 13.4 YouTube chapters

YouTube makes clickable chapters from timestamps in a video description, but
only if the rules are met. BeatFreaks follows them:

- The first chapter always starts at **0:00**.
- Chapters are at least **10 seconds** apart (closer tracks are skipped).
- There must be at least **3** chapters. If your list is shorter, no chapter
  text is made.

Copy the chapter lines into your video description, on their own lines.

## 13.5 CUE sheet

A CUE sheet is a small text file that sits next to your recorded mix (same
name, ending `.cue`). It lists each track with its start time. Players that
understand CUE files show the mix as separate tracks.

- It includes the mix title and your DJ name as the performer.
- It works with WAV and MP3 recordings.

## 13.6 CSV for spreadsheets

Columns: track number, start time, artist, title, key, BPM, and whether it was
AI-made.

For safety, any cell that starts with `=`, `+`, `-` or `@` is stored as plain
text, so a song title can never run as a formula in your spreadsheet.

## 13.7 Mix cover

SoundCloud and Mixcloud like a square cover image (3000 × 3000 pixels works
well). BeatFreaks plans the layout:

- **Default (no album art):** BeatFreaks' own original artwork with a title
  band across the bottom fifth of the image showing the mix title and a line
  like "DJ Name · date · 12 tracks".
- **With album art (only if you turn it on):** a grid of the album covers from
  the songs that were on air the longest: 1 cover, a 2 × 2 grid (4 covers) or
  a 3 × 3 grid (9 covers), plus the same title band.

### Album art is opt-in: read the rights note

Using the songs' album art on your mix cover is **off by default** and you
must turn it on yourself. Here is why:

- Owning (or having bought) a song gives you the right to play it. It does
  **not** give you the right to publish its cover artwork.
- Album art belongs to the artist, label or designer. Posting it on your mix
  cover may break their copyright or the platform's rules.
- When this feature has a screen, BeatFreaks will show a warning before you
  turn album art on.

Only turn album art on if you have permission, or if the platform and the
rights holders allow it. The safe choice is the default BeatFreaks cover.

This is general information, not legal advice.

![Screenshot: (planned) two example mix covers, the default BeatFreaks cover and a 2x2 album-art grid, both with the title band](images/mix-cover-examples-planned.png)

## 13.8 What you can do today

1. After your set, click **History** in the library sidebar and open today's
   list.
2. Right-click it → **Export Playlist**. Choose a format (for example M3U or
   CSV) and save it.
3. If you recorded with **Create a CUE file** on (chapter 8), the `.cue` file
   is next to your recording.
4. Write your post from that list by hand.

> **Coming later:** one-click publishing of a mix with its tracklist to
> Mixcloud or SoundCloud (only after checking their rules), and saving mixes,
> tracklists and covers to a cloud folder automatically.

Next: [Sending logs and feedback](14-sending-logs-and-feedback.md)
