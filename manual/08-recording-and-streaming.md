# 8. Recording and streaming

> **Status: Available now** (recording a mix and live broadcasting to an
> Icecast or Shoutcast server).
>
> **Coming later:** OBS integration (now-playing text, scene switches,
> start/stop stream), a stream overlay page, an Elgato Stream Deck plugin, and
> one-click publishing to Mixcloud or SoundCloud.

## 8.1 Before you record: set it up once

1. Click **Options → Preferences → Recording**.
2. **Directory**: where recordings are saved. The default is your **Music**
   folder, inside `Mixxx\Recordings`. Click **Browse...** to choose another
   folder.
3. **Output File Format**: pick one.

| Format | Type | Good for |
| --- | --- | --- |
| **WAV PCM** | Lossless (full quality), big files | Editing afterwards, best quality |
| **AIFF PCM** | Lossless, big files | Same as WAV, preferred on Apple devices |
| **FLAC** | Lossless, about half the size of WAV | Archiving at full quality |
| **MP3** | Lossy (smaller, some quality lost) | Uploading and sharing |
| **OGG Vorbis** | Lossy | Sharing |
| **Opus** | Lossy, very efficient | Sharing |
| **AAC** / **HE-AAC** | Lossy | Only listed if an AAC encoder library is installed on your PC |

4. For lossy formats, set **Quality** (higher = better sound, bigger file).
   For FLAC, set **Compression Level** (only changes size and speed, not
   quality).
5. **Channels**: stereo (normal) or mono.
6. **Tags**: type a **Title**, **Author** and **Album** to store in the file.
7. **Split recordings at**: start a new file after a set size (useful for very
   long sets with WAV).
8. **Create a CUE file**: also saves a `.cue` file listing each track and the
   time it started, so the mix can be split into tracks in many players.
   **Enable File Annotation in CUE file** adds each track's file location.
9. **Store the tracklist as comment** (MP3 and WAV only): writes the
   tracklist into the file's comment tag.
10. Click **OK**.

![Screenshot: Preferences, Recording page with directory, format and CUE options](images/prefs-recording.png)

## 8.2 Record a mix

1. Check your main level: the main meter should not hit red. The recording is
   taken from the main mix.
2. Click **REC** in the toolbar, or **Options → Record Mix**, or press
   **Ctrl + R**.
3. The REC button lights and shows the recording time.
4. DJ as normal.
5. Click **REC** again (or Ctrl + R) to stop.

Find your recording:

- In the library sidebar, click **Recordings**. Your mixes are listed there,
  and you can play them on a deck.
- Or open the recordings folder in Windows.

![Screenshot: toolbar REC button while recording, showing elapsed time](images/toolbar-recording.png)

Tips:

- Recording only captures what goes to the main output (including
  microphones, if routed to main).
- Use **Soundcard Clock** in Sound Hardware for live gigs (chapter 2).
- If you hear clicks in your recording, they were also in the main output.
  Increase the audio buffer (chapter 15).

> **In development:** BeatFreaks' own tracklist tools will turn a recorded
> mix into a numbered tracklist, YouTube chapters, a CUE sheet, a CSV file and
> a mix cover (chapter 13). Mixxx's built-in **Create a CUE file** option
> above already works today.

## 8.3 Live broadcasting

**Live broadcasting** sends your mix over the internet to a streaming
**server** (Icecast or Shoutcast). Listeners tune in with a web player or
radio app. You need an account on such a server (from an internet radio host,
or your own server). BeatFreaks does not provide a server.

> Broadcasting copyrighted music in public usually needs a licence. Check the
> rules for your country and your streaming host.

### Set up a connection

1. Click **Options → Preferences → Live Broadcasting**.
2. Under **Live Broadcasting source connections**, click **Create new
   connection**. Give it a name (you can **Rename selected** later).
3. With the connection selected, fill in **Settings for** that connection:

**Server connection** (your host gives you these):

| Field | What to enter |
| --- | --- |
| **Type** | **Icecast 2**, **Shoutcast 1** or **Icecast 1**, as your host says |
| **Host** | The server address |
| **Port** | The port number |
| **Mount** | The mount point (Icecast), for example `/live` |
| **Login** | User name (often `source`) |
| **Password** | Your stream password |
| **Password storage** | **Plain text** or **Secure storage (OS keychain)**. Choose secure storage if available |

**Encoding**: **Format** (**MP3**, **Ogg Vorbis**, **Opus**, or AAC types if
an AAC library is installed; use what your host supports), **Bitrate** (128 kbps is common; higher = better sound,
more upload needed), **Channels** (**Automatic**, **Mono** or **Stereo**).

**Stream info**: **Stream name**, **Website**, **Description**, **Genre**,
and **Public stream** (list it in the server's public directory or not).

**Metadata**: what listeners see as "now playing". **Format** can use
`$artist`, `$title`, `$year`, `$album`, `$genre` and `$bpm`. Or tick **Use
static artist and title** and type fixed text. Tick **Use UTF-8 encoding for
metadata** for accents and non-Latin scripts.

**Automatic reconnect**: tick **Enable automatic reconnect** and set the
**Reconnect period**, **Wait until first attempt** and **Maximum retries**
(or **Limit number of reconnection attempts**) so the stream comes back after
a short internet drop.

4. Click **OK**.

![Screenshot: Live Broadcasting preferences with one connection selected and server fields filled](images/prefs-broadcasting.png)

### Go live

1. Click **ON AIR** in the toolbar, or **Options → Enable Live
   Broadcasting** (Ctrl + L).
2. The button shows **...** while connecting, then **ON AIR**.
3. If it shows **ERROR**, check your host, port, mount and password, and your
   internet connection.
4. Click it again to stop.

You can set up several connections and use them at once (for example two
different radio servers). Use **Disconnect all** to stop them all.

### Streaming to video platforms

Platforms like YouTube or Twitch need a video stream, not an Icecast audio
stream. Today, use a separate program such as OBS Studio and select your
BeatFreaks output (or a loopback audio device) as its audio source.

> **Coming later:** direct OBS control from BeatFreaks (now-playing text on
> screen, switching scenes, starting and stopping the stream), a stream
> overlay page that never shows your library or the next track, and a Stream
> Deck plugin.

Next: [Auto DJ](09-auto-dj.md)
