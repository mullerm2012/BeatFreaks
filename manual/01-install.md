# 1. Installing BeatFreaks

> **Status: Available now** (test installers). A public download page and
> store are planned but not open yet.

This chapter covers what your PC needs, how to install, the Windows warning you
will see, how to test BeatFreaks safely in Windows Sandbox, and how to update
or remove it.

## 1.1 What your PC needs

| Part | Minimum | Better |
| --- | --- | --- |
| Windows | Windows 10 or Windows 11, 64-bit | Windows 11, fully updated |
| Memory (RAM) | 4 GB | 8 GB or more |
| Processor | Any recent dual-core 64-bit processor | Quad-core or better (helps with effects, key lock and four decks) |
| Graphics | Any graphics chip that supports OpenGL | A dedicated graphics card (only matters for AI songs, see chapter 12) |
| Disk space | About 1 GB for the program, plus room for your music and recordings | An SSD (fast disk) for your music |
| Sound | Your PC's built-in sound works for practice | A DJ controller with a built-in sound card, or a USB audio interface with two stereo outputs (one for speakers, one for headphones) |

What the words mean:

- **64-bit**: the modern kind of Windows. To check, open **Settings → System →
  About** and look at **System type**. It should say "64-bit operating
  system".
- **OpenGL**: a standard way for programs to draw graphics. BeatFreaks uses it
  to draw the moving waveforms. Almost every PC has it.

![Screenshot: Windows Settings, System, About page with System type highlighted](images/windows-about-64bit.png)

## 1.2 Download the installer

BeatFreaks comes as one file that ends in **.msi** (a Windows installer
package). The name looks like `beatfreaks-<version>-amd64.msi`.

Right now, test installers are shared by the BeatFreaks team directly. When the
public website opens, you will download it from there.

1. Download the `.msi` file.
2. Save it somewhere easy to find, such as your **Downloads** folder.
3. Do not rename it.

> **Safety tip:** only install BeatFreaks from a link the BeatFreaks team gave
> you or from the official website. A copy from somewhere else could be
> changed.

## 1.3 Run the installer

1. Open your **Downloads** folder.
2. Double-click the BeatFreaks `.msi` file.
3. If Windows shows a blue box that says **"Windows protected your PC"**, see
   the next section, then come back here.
4. The setup window opens. Click **Next**.
5. Read the licence (the GNU General Public License, see chapter 17). Tick
   **I accept the terms in the License Agreement** and click **Next**.
6. Keep the suggested install folder unless you have a reason to change it.
   Click **Next**.
7. Click **Install**.
8. If Windows asks **"Do you want to allow this app to make changes to your
   device?"**, click **Yes**.
9. Wait for the progress bar to finish, then click **Finish**.

You will find BeatFreaks in the **Start** menu.

![Screenshot: BeatFreaks setup window, welcome page with the Next button](images/installer-welcome.png)
![Screenshot: BeatFreaks setup window, licence page with the accept tick box](images/installer-licence.png)
![Screenshot: BeatFreaks setup window, finished page](images/installer-finish.png)

> **Note:** some early test installers still show the name "Mixxx" in the
> setup window or the Start menu. That is the same program before it was
> renamed.

> **Important if you also use the real Mixxx program:** an early test
> installer that still says "Mixxx" is treated by Windows as a new version of
> Mixxx. It will **replace** your Mixxx install and share its settings
> folder. Installers named "BeatFreaks" are kept separate: they never touch
> Mixxx and use their own settings folder. If you use both programs, only
> install a BeatFreaks-named installer.

## 1.4 The "Windows protected your PC" warning (SmartScreen)

Windows has a safety filter called **SmartScreen**. It warns you about new
programs that are not yet well known or not signed with a paid certificate.
BeatFreaks test installers are not signed yet, so you will probably see this
warning. It does not mean BeatFreaks is harmful.

1. When the blue box appears, click the small link **More info**.
2. A new line appears that shows the file name and "Publisher: Unknown
   publisher".
3. Click the **Run anyway** button that now appears.
4. The installer starts. Continue with step 4 in section 1.3.

![Screenshot: SmartScreen warning box with the More info link highlighted](images/smartscreen-more-info.png)
![Screenshot: SmartScreen box after clicking More info, with the Run anyway button highlighted](images/smartscreen-run-anyway.png)

Only do this for an installer you got from the BeatFreaks team or the official
website.

## 1.5 Testing safely in Windows Sandbox (Windows 11 Pro)

**Windows Sandbox** is a throwaway copy of Windows that runs in a window. When
you close it, everything inside it is deleted. It is a good way to try a test
version of BeatFreaks without touching your real setup.

Limits you should know:

- It needs **Windows 10 or 11 Pro, Enterprise or Education**. It is **not**
  available on Windows Home.
- USB controllers (like the S4 MK3 or F1) usually **do not** work inside the
  sandbox. Test controllers in normal Windows.
- Everything is erased when you close the sandbox, including your library and
  settings.

### Turn on Windows Sandbox (one time)

1. Click **Start**, type **Turn Windows features on or off**, and open it.
2. Scroll down and tick **Windows Sandbox**.
3. Click **OK**.
4. Restart the PC when Windows asks.

![Screenshot: Windows Features dialog with Windows Sandbox ticked](images/windows-features-sandbox.png)

If the **Windows Sandbox** line is greyed out, virtualisation (a processor
feature) is turned off in your PC's BIOS. Your PC maker's support pages explain
how to turn it on.

### Try BeatFreaks inside the sandbox

1. Click **Start**, type **Windows Sandbox**, and open it.
2. On your normal desktop, right-click the BeatFreaks `.msi` file and click
   **Copy**.
3. Click inside the sandbox window, right-click its desktop and click
   **Paste**.
4. Double-click the `.msi` inside the sandbox and install as in section 1.3.
5. Copy a few music files in the same way to test with.
6. When you are done, close the sandbox window. Everything inside is deleted.

![Screenshot: Windows Sandbox window with the BeatFreaks installer on its desktop](images/sandbox-installer.png)

## 1.6 Updating to a newer version

1. Close BeatFreaks.
2. Optional but wise: make a backup of your settings folder (see "Where your
   settings live" below). Copy the whole folder somewhere safe.
3. Run the new `.msi` installer the same way as before.
4. The installer replaces the old program files. Your library, playlists,
   crates, cue points and settings are kept, because they are stored in your
   settings folder, not in the program folder.
5. Start BeatFreaks. If the library is scanned again, let it finish.

> We recommend the backup because test versions can change how settings are
> stored. A copy lets you go back if something goes wrong.

## 1.7 Uninstalling

1. Close BeatFreaks.
2. Click **Start → Settings → Apps → Installed apps**.
3. Find **BeatFreaks** in the list (early test builds may be listed as
   **Mixxx**).
4. Click the **⋯** button next to it and click **Uninstall**. Confirm.

Uninstalling removes the program but **keeps** your settings folder, your
library database and your music files. To remove everything:

1. Uninstall as above.
2. Press **Windows key + R**, type `%LOCALAPPDATA%` and press **Enter**.
3. Delete the **BeatFreaks** folder (or **Mixxx** folder for very early test
   builds; only do this if you do not also use the real Mixxx program).

Your music files are never deleted by BeatFreaks.

## Where your settings live

BeatFreaks keeps your library database, cue points, playlists, crates,
settings, controller mappings you made, and log files in one **settings
folder**.

- Easiest way to open it: in BeatFreaks, click **Help → Settings directory**.
- In current BeatFreaks test builds the folder is `%LOCALAPPDATA%\BeatFreaks`.
  This keeps BeatFreaks separate from a normal Mixxx install on the same PC.
- Very early test builds (from before the name change) used
  `%LOCALAPPDATA%\Mixxx`, the same folder as Mixxx.

`%LOCALAPPDATA%` is a shortcut name Windows understands. It usually means
`C:\Users\<your name>\AppData\Local`.

![Screenshot: File Explorer showing the BeatFreaks settings folder with mixxx.log and mixxxdb.sqlite](images/settings-folder.png)

Next: [First run](02-first-run.md)
