# 14. Sending logs and feedback

> **Status:** finding and sending the log file by hand is **Available now**.
> A one-click **Help → Send log** report and a **Hardware Test** checklist are
> **In development** (the engines are written and tested, the menu items are
> not in the program yet). A tester discount is **Coming later** (planned for
> when BeatFreaks goes on sale; not final).

When something goes wrong, the **log file** is the most useful thing you can
send. It is a plain text diary that BeatFreaks writes while it runs: what it
loaded, which controllers and sound devices it found, and any warnings or
errors.

## 14.1 Where the log file is

BeatFreaks writes its log into your **settings folder**. The file is called
**`mixxx.log`** (BeatFreaks is built on Mixxx, so the file keeps that name).

The fastest way to open the folder:

1. In BeatFreaks, click **Help → Settings directory**.
2. A File Explorer window opens on the settings folder.
3. Look for `mixxx.log`.

If BeatFreaks will not start, open the folder by hand:

1. Press **Windows key + R**.
2. Type `%LOCALAPPDATA%\BeatFreaks` and press **Enter**.
3. If that folder does not exist, try `%LOCALAPPDATA%\Mixxx` (very early test
   builds, from before the name change, used that folder).

![Screenshot: File Explorer in the BeatFreaks settings folder with mixxx.log, mixxx.log.1 and mixxx.log.2 highlighted](images/settings-folder-logs.png)

### Which log file to send

Each time BeatFreaks starts, it renames the old log and starts a new one:

| File | What it holds |
| --- | --- |
| `mixxx.log` | The current run (or the last run, if BeatFreaks is closed) |
| `mixxx.log.1` | The run before that |
| `mixxx.log.2` … `mixxx.log.10` | Older runs (up to 10 are kept) |

So:

- **If BeatFreaks crashed or froze:** send `mixxx.log` **before** you start
  BeatFreaks again. If you already restarted it, send `mixxx.log.1` (the
  crashed run) as well.
- **If something went wrong but it is still running:** close BeatFreaks first
  (so everything is written), then send `mixxx.log`.
- When unsure, send `mixxx.log` and `mixxx.log.1`.

### What is in the log (privacy)

The log can contain:

- File paths, which include your Windows user name and folder names.
- Track names and artists you loaded.
- Names of your sound devices and controllers.

It does **not** contain passwords or your music files. If you do not want to
share your user name, open the log in Notepad and use **Edit → Replace** to
replace it with something like `USER` before sending.

BeatFreaks' own lines are tagged so they are easy to find, for example
`beatfreaks.library` (library and energy analysis), `beatfreaks.ai`,
`beatfreaks.hw.s4` and `beatfreaks.hw.f1`.

## 14.2 How to report a problem

Please include:

1. **What you did**, step by step ("I loaded a track on deck 2 and pressed
   SYNC on the S4").
2. **What you expected** to happen.
3. **What happened instead.**
4. **How often**: every time, sometimes, once.
5. **Your setup**:
   - BeatFreaks version (click **Help → About**).
   - Windows version (Settings → System → About).
   - Controller(s) and mapping name (Preferences → Controllers).
   - Sound API, device, sample rate and audio buffer (Preferences → Sound
     Hardware).
6. **The log file(s)** from section 14.1.
7. A **screenshot** or phone photo if it helps (press **Windows key + Shift +
   S** to take a screenshot).

Send it the way the BeatFreaks team told you when you received your test
version (for example the BeatFreaks issue page on GitHub at
`github.com/mullerm2012/BeatFreaks/issues`, if you have access).

> Please do **not** send BeatFreaks problems to the Mixxx project. BeatFreaks
> is a separate program, and the Mixxx team cannot help with BeatFreaks
> changes.

## 14.3 Extra logging for controller problems

If a controller button does the wrong thing, a more detailed log helps a lot.

1. Close BeatFreaks.
2. Right-click the BeatFreaks shortcut on your desktop (or in the Start menu:
   right-click → **More → Open file location**, then right-click the
   shortcut) and click **Properties**.
3. In the **Target** box, click at the very end (after the closing quote, if
   there is one), type a space, then `--controller-debug`.
4. Click **OK**.
5. Start BeatFreaks with that shortcut and repeat the problem. Every button
   press and knob turn is now written to the log.
6. Close BeatFreaks and send `mixxx.log`.
7. Remove `--controller-debug` again afterwards; it makes the log very large.

Other start options that can help, used the same way:

| Option | What it does |
| --- | --- |
| `--controller-debug` | Logs every message from controllers |
| `--safe-mode` | Turns off the OpenGL waveforms and spinning vinyl widgets. Try this if BeatFreaks crashes when starting |
| `--developer` | Adds the **Developer** menu and extra statistics (for advanced users) |

![Screenshot: shortcut Properties window with --controller-debug added to the Target box](images/shortcut-controller-debug.png)

## 14.4 Coming soon: Help → Send log (In development)

A **Send log** item in the Help menu will create one text report for you,
named `BeatFreaks-report-<date>-<time>.txt`, with:

- the BeatFreaks version and the settings folder location,
- the end of the newest log (about the last 4000 lines) and the end of the
  previous run's log (about 1000 lines),
- the connected controllers and their mappings,
- the sound devices, API, buffer and sample rate,
- the Hardware Test results (see below),
- a short summary of the open Arrange project.

To protect your privacy, every path inside your Windows user folder is
shortened to `~`, so your user name and personal folder names are not shared.

## 14.5 Coming soon: Hardware Test (In development)

A **Hardware Test** screen for the Traktor Kontrol S4 MK3 and F1 will list
every control. As you press each button and move each knob or fader, it is
ticked off. You can mark a control as broken (for example "LED did not
light"). The result is a checklist (passed, failed, never touched) that goes
into the Send log report.

## 14.6 Tester discount (Coming later)

BeatFreaks plans to reward early testers: a discount on the paid version for
people who send a useful log or bug report during testing. The amount and
rules are not final and will be announced with the store.

## 14.7 Ideas and feature requests

Ideas are welcome. Tell us:

- what you were trying to do,
- how you do it today (in BeatFreaks or another program),
- why it matters at a gig.

Next: [Troubleshooting](15-troubleshooting.md)
