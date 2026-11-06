# 7. Controllers

A **controller** is a hardware unit with buttons, knobs, faders and jog wheels
that controls BeatFreaks. A **mapping** is the file that tells BeatFreaks what
each control does.

| What | Status |
| --- | --- |
| Using a controller, choosing a mapping, mapping settings | **Available now** |
| 164 built-in mappings from Mixxx (Hercules, Numark, Pioneer, Native Instruments Traktor, Denon, Reloop, Behringer, Novation, Korg, Akai, Vestax and more) | **Available now** |
| Traktor Kontrol S4 MK3 mapping | **Available now** (from Mixxx). BeatFreaks testing on Windows, including the motorised platters and screens, is still going on. |
| Traktor Kontrol F1 mapping | **Available now** (from Mixxx, a basic two-deck mapping). BeatFreaks testing is still going on. |
| MIDI Learning Wizard | **Available now** (MIDI controllers only) |
| Hardware Test screen (every button lights up on screen) | **In development** |
| Pioneer DDJ-WeGO4 and Numark 4TRAK | **Coming later** (not supported yet) |
| BeatFreaks extras on controllers (Smart Next, crowd hot/cold, Autopilot "take a break") | **Coming later** |
| Friendly "learn any controller" wizard and on-screen button editor | **Coming later** |

## 7.1 Connecting a controller

1. Install the maker's driver if the maker provides one. For Native
   Instruments devices (S4 MK3, F1), install their driver from the Native
   Instruments website. Controllers with a built-in sound card (like the S4
   MK3) need the driver for their audio.
2. Plug the controller into a USB port **directly** on the PC if you can. Some
   USB hubs cause drop-outs.
3. Switch the controller on (if it has a power switch or adapter: the S4 MK3
   needs its power adapter for the motorised platters).
4. Start BeatFreaks.

BeatFreaks looks for controllers when it starts. If you plug one in later,
restart BeatFreaks.

## 7.2 Turning it on and choosing a mapping

1. Click **Options → Preferences**.
2. Click **Controllers** on the left. Your controller is listed under it. If
   it is not, see chapter 15, section 15.4.
3. Click your controller's name.
4. In **Load Mapping**, choose the mapping for your model. Mappings with the
   same name as your controller are the right ones.
5. Tick **Enabled**.
6. Click **Apply**, then **OK**.

The controller's lights should come on and react.

![Screenshot: Preferences, Controllers page with Traktor Kontrol S4 MK3 selected, Load Mapping and Enabled](images/prefs-controller-s4.png)

Other things on that page:

- **Device Info**: the physical connection (USB HID, MIDI or USB bulk),
  vendor and product names and IDs. Useful when reporting a problem.
- **Mapping Info**: name, author, description and a **Support** link.
- **Mapping Files**: the files the mapping uses.
- **Mapping Settings**: options for this mapping (see the S4 MK3 settings in
  section 7.4). Change, then click **Apply**.
- **Screens preview**: for controllers with screens.
- **Input Mappings** / **Output Mappings**: the raw list of what each control
  does (MIDI controllers). You can **Add**, **Remove** or **Clear All**.
- **Learning Wizard (MIDI Only)**: see section 7.7.
- **Refresh mapping list**: reloads the list if you added mapping files.

Some controllers show up **twice** (for example the S4 MK3 can show a second
entry for its screens). Each entry gets its own mapping.

## 7.3 Traktor Kontrol S4 MK3 overview

The S4 MK3 is a four-deck controller with a built-in sound card, two
motorised jog wheels with LED rings, two colour screens, eight pads per side,
two effect units and a four-channel mixer.

The BeatFreaks mapping is Mixxx's own S4 MK3 mapping. When you enable it:

- BeatFreaks switches on **four decks** and **16 samplers** automatically.
- The left side controls **deck 1 (A)** or **deck 3 (C)**; the right side
  controls **deck 2 (B)** or **deck 4 (D)**.
- Default deck colours: A red, B blue, C yellow, D purple (change them in the
  mapping settings).

> **Testing status:** the S4 MK3 mapping comes from Mixxx and works there. The
> BeatFreaks team is still checking it on Windows with real hardware,
> including the motorised platters and the screens. Please report anything
> that does not match this chapter (chapter 14).

![Screenshot: photo-style diagram of the S4 MK3 with the deck, mixer, FX and library areas outlined](images/s4mk3-areas.png)

### Sound setup for the S4 MK3

In **Preferences → Sound Hardware** (chapter 2): Sound API **ASIO** if
available, **Main** = S4 MK3 channels 1–2, **Headphones** = S4 MK3 channels
3–4.

The S4 MK3's **MAIN** (master) volume, **BOOTH** volume, headphone **VOL** and
**CUE MIX** knobs work **inside the S4's own sound card**. They are not sent to
BeatFreaks by default. That is what you want when the speakers and headphones
are plugged into the S4. If you use a different sound card, turn on the
"…handled by the Mixxx built-in mixer" settings (section 7.4).

## 7.4 S4 MK3: every control

How to read the tables: "press" is a quick press, "hold" is a press longer
than about a quarter of a second, and "SHIFT +" means hold that side's
**SHIFT** button first. Each deck side has its own SHIFT button.

### Deck: transport

| Control | Press | SHIFT + |
| --- | --- | --- |
| **PLAY** | Play / pause | Hold SHIFT + hold PLAY (almost one second): stop and **eject** the track |
| **PLAY** (hold, deck stopped) | Copies (clones) a playing track onto this deck | — |
| **CUE** | Cue, following your cue mode (chapter 5) | Jump to the start of the track and stop |
| **SYNC** | Press: turn sync lock on/off. Hold: match tempo once | Press: **key lock** on/off. Hold: match key (sync key) |
| **MASTER** | Press: make this deck the sync leader (on/off). Hold: switch the tempo fader range between ±8% and ±100% | (Only if "Keylock on SHIFT+MASTER" is chosen in settings: key lock on/off) |
| **Tempo fader** | Changes speed. Snaps to 0% in a small centre zone. The LED beside it shows when the fader position and the real speed differ (soft takeover: move the fader through the real value to pick it up) | — |
| **Deck buttons** (left side **A** / **C**, right side **B** / **D**) | Switch which deck this side controls | — |

### Deck: jog wheel

| Control | What it does |
| --- | --- |
| **Jog wheel** (touch the top) | In vinyl mode: scratch. Let go to continue |
| **Jog wheel** (turn the side, or turn in jog mode) | Nudge (pitch bend) to line up beats |
| **JOG** button | Switches between **vinyl mode** (top touch scratches; lit) and **jog mode** (the wheel only nudges) |
| **TT** button | Turns the **motor** (turntable mode) on and off, **only** if "Allow Mixxx to interact with jog wheel motors (BETA)" is ticked in the settings. Otherwise it does nothing |
| LED ring | Shows the play position like a record spinning. Blinks near the end of the track (end-of-track warning) |

**Motorised platters (BETA, off by default).** With motors allowed, the platter
turns with the music and you can hold it like a record. Settings: **Jogwheel
tension**, **Jogwheel torque**, **Turntable speed** (33⅓ or 45 RPM feel), and
**Emit haptic feedback when the platter goes over the main cue**. The mapping
warns that motor use can cause extra wear on the device. Use at your own risk.

### Deck: FLUX, REV and GRID

| Control | Press / hold | SHIFT + |
| --- | --- | --- |
| **FLUX** | While held: **slip** mode (the track keeps running underneath; let go and it continues where it would have been) | Press: **loop out** (if no loop is active). If a loop is active: the jog wheel now moves the **loop end**; press again to leave this mode |
| **REV** | While held: plays backwards with slip (a "censor" effect); let go and it continues in time | Press: **loop in** (if no loop is active). If a loop is active: the jog wheel now moves the **loop start**; press again to leave |
| **GRID** | Press: moves the nearest beat-grid line to the current play position | — |
| **GRID** held + turn left encoder | Shifts the whole beat grid earlier or later | SHIFT + GRID held + turn left encoder: makes the grid faster or slower (fixes a wrong BPM) |
| **GRID** held + turn library encoder | Zooms the waveform in or out | — |

In loop-edit mode (SHIFT + REV or SHIFT + FLUX with a loop active), the
**right encoder** moves the whole loop, and with SHIFT it moves it in larger
steps.

### Deck: the two encoders above the pads

| Control | Turn | Press | SHIFT + turn | SHIFT + press |
| --- | --- | --- | --- | --- |
| **Left encoder** | Beat jump back / forward | Hold while turning: halve / double the beat-jump size. Also shows the size on the screen | Fine key (pitch) adjust down / up | Reset the key |
| **Right encoder** (loop) | Halve / double the loop size | Start a beat loop of that size | Move the loop back / forward by one beat | Loop on / off (reloop) |

### Deck: pads and pad modes

There are eight pads per side (two rows of four) and five pad-mode buttons:
**HOTCUE**, **RECORD**, **SAMPLES**, **MUTE** and **STEMS**.

**Default layout** (when no mode button is lit):

| Top row | Pad 1 | Pad 2 | Pad 3 | Pad 4 |
| --- | --- | --- | --- | --- |
| Function | Intro start | Intro end | Outro start | Outro end |

| Bottom row | Pad 5 | Pad 6 | Pad 7 | Pad 8 |
| --- | --- | --- | --- | --- |
| Function | Hotcue 1 | Hotcue 2 | Hotcue 3 | Hotcue 4 |

- Press an unlit pad to set that marker or hotcue. Press a lit one to jump.
- **SHIFT + pad**: delete that marker or hotcue.
- You can choose a different default layout in the mapping settings
  (**Default Pad Layout**: Intro/Outro + 4 hotcues, Hotcues, Sampler or
  beatloop, or Keyboard).

**Pad modes:**

| Mode button | What the pads do |
| --- | --- |
| **HOTCUE** | Pads = hotcues **1–8**. Press HOTCUE again to go back to the default layout. SHIFT + pad deletes a hotcue |
| **SHIFT + HOTCUE** | Pads = an extra page of hotcues (hotcues 13–20 in the current mapping) |
| **HOTCUE** held | Change hotcue colours: keep HOTCUE held, hold a pad, and turn the left encoder |
| **RECORD** | Pads = **beat jump** pads. Press = jump forward by that pad's size; SHIFT + pad = jump back. Sizes are set in the mapping settings (**Beatjump Size**) |
| **SAMPLES** | Pads = samplers (left side: samplers 1–4 and 9–12; right side: 5–8 and 13–16). Empty pad: loads the track selected in the library into that sampler. Loaded pad: plays it from its start. SHIFT + pad: stops it if playing, otherwise ejects it. The sampler row is shown on screen while this mode is on. Press SAMPLES again to go back |
| **MUTE** | No function in this mapping |
| **STEMS** | For **stem files** only (tracks split into drums, bass, melody and vocals, such as `.stem.mp4`). Top row: hold a pad to select a stem (then the left encoder sets its volume, and the right encoder controls its quick effect: turn to change, press to switch on/off). SHIFT + top-row pad: mute that stem. Bottom row: mute stems 1–4 |
| **SHIFT + STEMS** | **Keyboard mode**: pads play the track's cue at different musical keys, like a small keyboard. Turn the left encoder to move the range |

There is an option to use the **SAMPLES** page as **beatloop roll** pads
instead (**Use the Sampler tab as a Beatloop Roll tab instead**). Then each pad
plays a short rolling loop while held (sizes set under **Beatloop Roll
Size**).

![Screenshot: S4 MK3 pad area with default layout labels on each pad](images/s4mk3-pads-default.png)

### Library (browse) section

Each side has a **library encoder** (the browse knob) and four buttons:
**PREVIEW** (play symbol), **STAR**, **LIST** and **VIEW**.

| Control | Action |
| --- | --- |
| Library encoder, turn | Move up/down the track list |
| SHIFT + turn | Move up/down the sidebar (playlists, crates…) |
| **TT** held + turn | Scroll a page at a time |
| Press | Load the selected track into this side's deck (in the sidebar: open the selected item) |
| SHIFT + press | Expand or collapse the selected sidebar item |
| **PREVIEW** held | Plays the selected track in the preview deck (headphones) while held |
| **PREVIEW** held + turn | Skip 16 beats forward/back in the preview |
| **PREVIEW** held + hotcue pad | Use hotcues on the preview |
| SHIFT + **PREVIEW** | Copies the current deck's track into the preview deck |
| **STAR** press | Change the selected track's colour (SHIFT: the other way) |
| **STAR** held + turn | Change the selected track's star rating |
| **LIST** press | Open the selected track's right-click menu (then use the encoder to choose and press to confirm) |
| **LIST** held | Clear the search box |
| **VIEW** press | Big library on/off |
| **VIEW** held + turn | Choose which column to sort by (set the six columns in the mapping settings under **Library**) |
| **VIEW** held + press encoder | Reverse the sort order |
| An effect button held + turn | Change that effect (see FX below) |

### Mixer (four channels)

Each channel column:

| Control | Action |
| --- | --- |
| **GAIN** | Channel gain (trim) |
| **HI / MID / LOW** | EQ |
| **FILTER** knob | The channel's quick effect (normally the filter) |
| **FILTER** (quick effect) button | Press: quick effect on/off. Hold: on only while held |
| **FX assign 1** / **FX assign 2** | Send this channel to effect unit 1 or 2 (press: toggle; hold: only while held) |
| **CUE** (headphone) | Headphone cue on/off |
| Channel fader | Volume |
| Crossfader assign switch | Left / centre (thru) / right |
| Gain-save button | Stores the current gain as the track's ReplayGain level |

When a channel has a microphone or auxiliary input set up (chapter 2), holding
**SHIFT** makes that channel's gain, fader, CUE, crossfader switch and FX
assign control the mic or aux instead (setting **Shift to mix microphones or
auxiliary lines**, on by default).

Centre of the mixer:

| Control | Action |
| --- | --- |
| **FX SELECT** buttons (**FILTER**, **1**, **2**, **3**, **4**) | Load a quick effect preset onto **all four** channels' FILTER knobs. Press two buttons together to reach more presets (up to 25 combinations). To load a preset onto one channel only, hold an FX SELECT button and press that channel's FILTER button |
| **QUANTIZE** | Quantize on/off for all four decks |
| **Crossfader** | Crossfader |
| Crossfader curve switch | Three positions: fast cut, constant power, additive (smooth) |
| **MAIN**, **BOOTH**, headphone **VOL**, **CUE MIX** | Work inside the S4's sound card (see 7.3) |

### Effect units (two, at the top)

Effect unit 1 is on the left, unit 2 on the right. Each has four knobs and
four buttons.

| Control | Press / turn | SHIFT + |
| --- | --- | --- |
| First knob | Unit **mix** (dry/wet) | — |
| Knobs 2–4 | Meta knob of effects 1–3 | In focus mode: back to meta knobs |
| First button (unit button) | While held: turns all three effects on | Press: put this unit on the main output on/off; in focus mode: leave focus mode |
| Buttons 2–4 | Effect 1–3 on/off (hold: on only while held) | Press: load the next effect in that slot. **Hold**: **focus** that effect: the three knobs control its parameters 1–3 and the buttons its button parameters |
| Effect button held + library encoder | Pick the next/previous effect for that slot | — |

### Screens

The S4 MK3's screens use a separate mapping, **Traktor Kontrol S4 MK3
(Screens)**, on the second S4 entry in the controller list. It shows deck
information on the device. Its settings include **Theme use for the screen**,
an **Idle deck background** picture, an **Accent color**, **Decks layout**,
and what the tempo overlay shows.

> **Testing status:** the screens use a direct USB connection. Whether they
> work on Windows together with the Native Instruments driver is being tested.

### S4 MK3 mapping settings

In **Preferences → Controllers → Traktor Kontrol S4 MK3 → Mapping Settings**:

| Group | Settings |
| --- | --- |
| **Deck Lighting** | Colour for decks A–D; keep one-colour LEDs dimmed when inactive; make **GRID** blink on each beat (helps when fixing grids); keep both deck-select buttons lit; wheel LED blinks near the end of the track; tempo fader soft-takeover colours |
| **Tempo Fader** | Size of the centre "snap to 0%" zone in millimetres; centre offset for each side (if 0% is not at the printed centre) |
| **Alternative Mapping** | Key lock on SHIFT + MASTER instead of SHIFT + SYNC; use the SAMPLES page as beatloop rolls; put the first quick effect on FX SELECT **FILTER** instead of **1**; default pad layout |
| **Mixer** | SHIFT controls mic/aux; let the MAIN, BOOTH and headphone knobs also drive BeatFreaks' own mixer (only when you do **not** use the S4's outputs; can cause clipping otherwise) |
| **Beatloop Roll Size** | Size of each of the 8 roll pads |
| **Beatjump Size** | Size of each of the 8 beat jump pads |
| **Library** | The six columns that VIEW + encoder steps through |
| **Jog wheel** | Sensitivity when moving loops with the wheel or encoder; turntable speed |
| **Haptic drive feature (BETA)** | Allow motors; haptic bump at the main cue; tension; torque |

![Screenshot: S4 MK3 Mapping Settings in Preferences](images/s4mk3-mapping-settings.png)

## 7.5 Traktor Kontrol F1

The F1 has a 4×4 grid of 16 colour pads, four knobs, four faders, four buttons
under the faders, a browse encoder, a two-digit display and several function
buttons.

The BeatFreaks F1 mapping is Mixxx's original F1 mapping. It is **simple**:
it controls **decks 1 and 2** and **samplers 1–4**, and it has two modes.

- **CAPTURE** button: **Decks mode** (the default when it starts).
- **QUANT** button: **Samplers mode**.
- The lit button shows which mode is on.

The tables below number pads 1–16 from the top-left, left to right, row by
row. (This order is being confirmed on real hardware.)

### Decks mode (CAPTURE)

| Control | Action |
| --- | --- |
| Knob 1 | Headphone volume |
| Knob 2 | Headphone mix (cue ↔ main) |
| Knob 3 | Main balance (left/right) |
| Knob 4 | Main volume |
| Fader 1 | Deck 1 tempo |
| Fader 2 | Deck 1 volume |
| Fader 3 | Deck 2 volume |
| Fader 4 | Deck 2 tempo |
| Button under fader 1 | Deck 1 play/pause |
| Button under fader 2 | Deck 1 cue |
| Button under fader 3 | Deck 2 play/pause |
| Button under fader 4 | Deck 2 cue |

Pads in decks mode:

| Row | Pad (col 1) | Pad (col 2) | Pad (col 3) | Pad (col 4) |
| --- | --- | --- | --- | --- |
| 1 | Deck 1 hotcue 1 | Deck 1 hotcue 2 | Deck 2 hotcue 1 | Deck 2 hotcue 2 |
| 2 | Deck 1 hotcue 3 | Deck 1 hotcue 4 | Deck 2 hotcue 3 | Deck 2 hotcue 4 |
| 3 | Deck 1 loop in | Deck 1 loop out | Deck 2 loop in | Deck 2 loop out |
| 4 | Deck 1 quantize on/off | Deck 1 reloop/exit | Deck 2 quantize on/off | Deck 2 reloop/exit |

### Samplers mode (QUANT)

| Control | Action |
| --- | --- |
| Knobs 1–4 | Speed of samplers 1–4 |
| Faders 1–4 | Level (gain) of samplers 1–4 |
| Buttons under faders 1–4 | Play/pause samplers 1–4 |
| Pads, row 1 to row 4 | Hotcues 1 to 4; column 1 = sampler 1, column 2 = sampler 2, and so on |

### In both modes

- **SHIFT + hotcue pad**: delete that hotcue.
- **Browse encoder**: turn to move through the track list; press to load the
  selected track into the first stopped deck.
- When the F1 starts, the display runs a short test pattern.
- **SYNC**, **REVERSE**, **TYPE**, **SIZE** and **BROWSE** buttons have no
  function in this mapping.

> **Watch out:** in decks mode the F1's faders control deck 1 and 2's volume
> and tempo directly, with no "pick-up". If you also use an S4 MK3, moving an
> F1 fader makes the value jump to the F1 fader's position. Keep the F1 in
> **Samplers mode** while you mix on the S4, or set its faders to match first.

> **Coming later:** a BeatFreaks F1 layer built for sound effects and sample
> pads, DJ-tag macros, and "crowd hot / crowd cold" pads for Smart Next.

![Screenshot: F1 diagram with decks-mode labels on knobs, faders and pads](images/f1-decks-mode.png)
![Screenshot: F1 diagram with samplers-mode labels](images/f1-samplers-mode.png)

## 7.6 Using several controllers at once

BeatFreaks can use more than one controller at the same time, for example an
S4 MK3 for mixing and an F1 for samples.

1. Plug in all controllers before starting BeatFreaks.
2. In **Preferences → Controllers**, select and enable each one with its own
   mapping.
3. Only one device can be the main sound card. Use the S4 MK3 (or your audio
   interface) for sound; the F1 has no sound card.

Tips:

- Two controllers that both move the same thing (for example deck 1 volume)
  will fight. Plan which controller does what.
- Use the F1 in **Samplers mode** next to an S4 MK3 (see the warning above).
- A full test of S4 + F1 + other devices together is planned by the BeatFreaks
  team.

## 7.7 MIDI Learning Wizard (MIDI controllers only)

If your controller is a **MIDI** device and has no mapping, you can teach
BeatFreaks what each control does. (The S4 MK3 and F1 are **HID** devices, a
different kind of USB connection, so they cannot use the wizard.)

1. Open **Preferences → Controllers** and click your controller.
2. Tick **Enabled** and click **Apply**.
3. Click **Learning Wizard (MIDI Only)**. The **Controller Learning
   Wizard** opens.
4. Click **Learn**, then **Choose Control...** and pick a function (for
   example deck 1's play button). You can also click the control in the main
   window.
5. Move or press the matching control on your controller.
6. The wizard confirms it. Click **Learn Another** for the next control, or
   **Retry** if it caught the wrong one.
7. For knobs and faders, **Advanced MIDI Options** has **Soft Takeover**
   (ignore the knob until it reaches the current value), **Invert**, **Switch
   Mode** and **Jog Wheel / Select Knob**.
8. Click **Done**, then **Close**.
9. Back in Preferences, BeatFreaks tells you the **mapping has been edited**
   and asks if you want to save it. Enter a name. It is saved in your user
   folder.

Your learned mappings are saved in your settings folder under `controllers`.

> **Coming later:** a friendlier BeatFreaks wizard that walks you through
> "move the left fader… now press play…" and an on-screen button editor where
> you tap a control on a drawing of your controller and pick its job.

## 7.8 Controllers not supported yet

| Controller | Status |
| --- | --- |
| **Pioneer DDJ-WeGO4** | **Coming later.** Not in Mixxx's mapping list. A BeatFreaks mapping is planned, built from the device's own signals. It is not available yet. |
| **Numark 4TRAK** | **Coming later.** Not in Mixxx's mapping list (Mixxx has mappings for related Numark NS6II and NS7 models, which are **not** the same device). A BeatFreaks mapping is planned. |

If your controller is not in the list, you can still try:

1. A mapping for a very similar model (it may work partly).
2. The MIDI Learning Wizard (MIDI devices only).

Please tell us which controller you use (chapter 14). Popular models without a
mapping are being researched.

## 7.9 Hardware Test screen (In development)

BeatFreaks is getting a **Hardware Test** screen for the S4 MK3 and F1: every
button, knob and fader lights up on a drawing when you touch it, and you get a
pass/fail checklist to send with your log. The checklist engine is written and
tested; the screen and menu item are **not** in the program yet.

Until then, the controller page's **Input Mappings** list and the log file
(chapter 14) are the ways to check a controller.

Next: [Recording and streaming](08-recording-and-streaming.md)
