# 6. Loops, cues and effects

> **Status: Available now**

## 6.1 Hotcues

A **hotcue** is a saved spot in a track. Press its button and the track jumps
there instantly. Hotcues are saved with the track, so they are still there next
time.

On screen each deck shows numbered hotcue buttons (8 by default; the skin
settings can show more or fewer).

| Action | How |
| --- | --- |
| Set a hotcue | Click an empty (unlit) hotcue button. It is set at the current play position. |
| Jump to it | Click a lit hotcue button. |
| Edit (name, colour) or delete | Right-click the hotcue button. A menu opens. |
| Delete quickly | Hold **Shift** on the computer keyboard and right-click the hotcue (or, with keyboard shortcuts on, Shift + Z/X/C/V for deck 1). |
| Move to another number | Drag the hotcue button onto another hotcue button. If that one is set, the two swap. |
| Save a loop as a hotcue | Click an empty hotcue while a loop is active. It becomes a **loop cue**: pressing it later turns the loop on. |

With **Quantize** on, new hotcues snap to the nearest beat. Turn it on per
deck with the **quantize** button.

Hotcue colours come from **Preferences → Colors**: **Hotcue palette**,
**Hotcue default color**, **Loop default color** and **Jump default color**.
The same page has the **Track palette** and **Enable Key Colors** (colour
hints for keys).

While a hotcue is held (previewing), drag it onto the **Play** button to keep
playing after you let go.

![Screenshot: deck with four hotcues set in different colours and the right-click hotcue menu open](images/hotcues.png)

## 6.2 Intro and outro markers

Besides hotcues, each track can have four special markers:

- **Intro start** and **Intro end**: where the beat-only part at the start
  begins and ends.
- **Outro start** and **Outro end**: the same at the end.

They help you see where you can mix, and **Auto DJ** uses them to time its
fades (chapter 9).

- Show the buttons with the skin settings (or use them on a controller: the S4
  MK3's default pad layout has them, chapter 7).
- Click an empty marker button to set it. Click a set one to jump there.
  Right-click to clear it.
- **Preferences → Decks → Set intro start to main cue when analyzing tracks**
  places the intro start automatically.

> Automatic phrase detection that places these markers on 8, 16 and 32-bar
> boundaries is part of the planned **Autopilot**. Status: **Coming later**.

## 6.3 Loops

A **loop** repeats a part of the track. Loops are measured in **beats**, so
they stay in time.

### Beat loop (fastest way)

1. Choose a size with the **halve** and **double** arrows (for example 4 or 8
   beats).
2. Click the **beatloop** button. The loop starts from the current position.
3. Click **reloop** (or the beatloop button again, depending on skin) to leave
   the loop. The track carries on.

**Loop roll**: right-click the beatloop button. The loop plays only while you
hold it. When you let go, the track continues as if it had never looped (like
slip mode).

### Manual loop

1. Click **Loop in** where you want the loop to start.
2. Click **Loop out** where you want it to end. The loop starts at once.
3. Click **Reloop** to turn it off or on again.

Right-click **Reloop** to switch the loop on, jump to its start and stop.

### Changing a loop

- **Halve** / **double**: make the loop shorter or longer while it plays.
- **Loop move**: shift the whole loop forward or back by a beat.
- Save it: click an empty hotcue while it is active (loop cue).

With **Quantize** on, loop in/out snap to the beat grid. If the track has no
beat grid, sizes are in seconds instead.

![Screenshot: deck loop section with beatloop size, halve, double, loop in, loop out and reloop](images/loop-controls.png)

## 6.4 Beat jump

**Beat jump** moves the play position forward or back by a number of beats,
without stopping.

- Set the size with the arrows next to it (for example 16 or 32 beats).
- Click **back** or **forward**.
- While a loop is active, the same buttons **move the loop** by that many
  beats instead of jumping.
- Keyboard: **A** / **S** (deck 1), **J** / **K** (deck 2). With **Shift**,
  the jump is one beat.

Use it to skip a long breakdown or to line up phrases.

## 6.5 Slip mode

When **Slip** is on, the track keeps "playing silently" underneath while you
loop, scratch, jump or play backwards. When you stop, the music continues
from where it would have been. It keeps your mix in time with the crowd.

## 6.6 Effects

### What an effect unit is

BeatFreaks has **effect units** (two shown by default; four can be shown in
the skin settings). Each unit holds **three effects** in a row (a **chain**).

Each unit has:

- Three **effect slots**. Each slot has an on/off button, a selector to pick
  the effect, and a **meta knob** (one knob that controls the most useful
  setting of that effect).
- **Mix** knob: how much effect you hear. Left = none (dry), right = full
  effect (wet).
- **Super** knob: moves several effect settings at once, for a big sweep.
- **Assign** buttons, one per deck (and for headphones and main): which audio
  goes through this unit.
- A **chain preset** selector: load a saved combination of three effects.

Click the little arrow or **expand** button on a slot to see every setting
(parameter) of that effect.

### Using an effect, step by step

1. Show the effects: click **EFFECTS** in the toolbar.
2. In **unit 1**, click the assign button for **deck 1** so it lights up.
3. In the first slot, choose an effect, for example **Echo**.
4. Turn the slot on.
5. Turn the unit's **Mix** knob up slowly. You now hear the echo on deck 1.
6. Turn the slot's **meta** knob to change the effect.
7. Turn the slot off or the Mix knob down to stop.

![Screenshot: effect unit with deck 1 assigned and Echo loaded in slot 1](images/effect-unit-echo.png)

### Built-in effects

The list includes: **Echo**, **Reverb**, **Filter**, **Moog Ladder 4
Filter**, **Flanger**, **Phaser**, **Bitcrusher**, **Glitch**, **Autopan**,
**Tremolo**, **Distortion**, **Pitch Shift**, **Compressor**, **Auto gain
control**, **Loudness Contour**, **Balance**, **Graphic Equalizer**,
**Parametric Equalizer**, **White Noise**, **Metronome**, and several EQ types
used by the mixer.

Many time-based effects (Echo, Flanger, Glitch and others) have a
**Quantize** option and sync to the track's tempo.

### Effect preferences

**Preferences → Effects**:

- **Effect Chain Presets**: drag to reorder; **Import**, **Rename**,
  **Export**, **Delete**. Create and edit presets in the effect units in the
  main window.
- **Quick Effect Chain Presets**: the list the quick effect knob and the S4
  MK3's FX SELECT buttons pick from, in this order.
- **Visible Effects** / **Hidden Effects**: drag to show or hide effects in the
  lists.
- **Effect load behavior**: **Keep metaknob position** or **Reset metaknob to
  effect default** when you swap an effect.

### Quick effect knob

Each mixer channel's quick effect knob (normally a filter, chapter 5) can be
changed per deck: click the small selector next to it and pick another quick
effect chain.

> **Coming later:** BeatFreaks "transition recipes" (one-press blend, filter
> rise, echo out, loop roll, double drop, spinback, riser, tempo ramp) and
> extra effects such as tape stop and a beat masher. None of these are in the
> program yet.

## 6.7 Samplers

**Samplers** are small extra decks for short sounds: an air horn, a vocal
shout, your DJ name tag, a drum loop.

- Show them: click **SAMPLERS** in the toolbar.
- Load: drag a file onto a sampler.
- Play: click the sampler's play button. Each sampler also has hotcues, a
  volume knob, an orientation (crossfader side), a sync button and more,
  depending on the skin and size.
- Samplers follow the **sync** and **key lock** settings, so a loop can stay
  in time with your track.
- **Save a sampler bank**: open the skin settings panel (**SETTINGS** in the
  toolbar). In the sampler options there are buttons to **save** and **load**
  a sampler bank (all sampler contents at once).

The S4 MK3 and F1 can play samplers from their pads (chapter 7).

> **Coming later:** a BeatFreaks sample-pad layer for the F1 designed for
> sound effects, and one-press DJ-tag macros.

## 6.8 Preview deck

A small extra deck just for listening in headphones.

1. Click **View → Show Preview Deck**.
2. Drag a track onto it, or use the play button that appears in the library's
   **Preview** column.
3. It plays only in your headphones, not to the crowd.

Next: [Controllers](07-controllers.md)
