# 5. Decks and mixing

> **Status: Available now**

This chapter covers the basics of playing and blending two tracks: loading,
play, cue, sync, tempo, key lock, EQ, the crossfader and headphone cueing. On a
controller the same actions are on hardware buttons (chapter 7).

## 5.1 Two decks or four

BeatFreaks has four decks. Most people start with two.

- Click **4 DECKS** in the toolbar to show decks 3 and 4.
- Decks 1 and 3 are on the left, 2 and 4 on the right.
- Some controllers (like the S4 MK3) turn four decks on automatically.

## 5.2 Load a track

1. Find the track in the library (chapter 4).
2. Drag it onto a deck, or double-click it, or select it and press **Shift +
   Left Arrow** (deck 1) / **Shift + Right Arrow** (deck 2) with keyboard
   shortcuts on.

What happens:

- The waveform appears and the title, artist, BPM and key show.
- If the track was never analysed, BeatFreaks analyses it now. The BPM and key
  appear after a few seconds.
- The deck jumps to the **load point**: by default the main cue point.

Change the load point in **Preferences → Decks → Track load point**:
**Intro start**, **Main cue**, **First hotcue**, **First sound (skip
silence)** or **Beginning of track**.

### Loading onto a deck that is playing

BeatFreaks can protect you from replacing a playing track by mistake.
**Preferences → Decks → Loading a track, when deck is playing** lets you
choose:

- **Reject** (nothing happens; the safest choice for live sets),
- **Allow, but stop deck**,
- **Allow, play from load point**.

### Clone a deck

To copy the playing track (at the same position) to another deck:

- Drag from one deck's waveform or title onto the other deck, or
- Turn on **Preferences → Decks → Double-press Load button to clone playing
  track** and double-press load on a controller or keyboard.

## 5.3 Play, pause and cue

- **Play/Pause**: click the triangle button.
- **Cue**: the **main cue point** is your starting spot.

How the **Cue** button behaves depends on **Preferences → Decks → Cue mode**:

| Cue mode | Pressing Cue while stopped | Pressing Cue while playing |
| --- | --- | --- |
| **Mixxx mode** (default) | Sets the cue point here (at the cue point, holding it previews) | Goes back to the cue point and stops |
| **Mixxx mode (no blinking)** | Same as Mixxx mode, but the button does not blink | Same |
| **Pioneer mode** | Sets the cue point (like Pioneer players) | Goes back to the cue point and stops |
| **Denon mode** | Previews from the cue point (like Denon players) | Goes back to the cue point and stops |
| **Numark mode** | Sets the cue point (like Numark players) | Goes back to the cue point and stops |
| **CUP mode** ("Cue And Play") | Sets the cue point and starts playing when you let go | Goes to the cue point and plays when you let go |

If you are used to a specific brand's players, pick that mode. Hover the Cue
button to read the exact behaviour of your chosen mode.

Other helpful cue tricks:

- **Right-click Play** sets the cue point at the current spot.
- **Right-click Cue** jumps to the cue point and stops.
- Hover the buttons to see every option for your cue mode.

![Screenshot: Preferences, Decks page with Cue mode list open](images/prefs-decks-cue-mode.png)

## 5.4 Matching tempo: sync

To mix two tracks smoothly, they must play at the same speed (BPM) and their
beats must line up.

### The SYNC button

- **Tap SYNC once**: the deck jumps to the tempo of the other playing track
  (or the sync leader) one time.
- **Hold SYNC for at least one second**: turns on **sync lock**. The button
  stays lit. From now on the deck always follows the leader's tempo. If
  **Quantize** is also on, the beats stay lined up too.
- **Tap SYNC again** while it is lit to turn sync lock off.

![Screenshot: deck tempo section with SYNC, sync leader (crown) and key lock buttons highlighted](images/deck-sync-keylock.png)

### The sync leader

The **leader** is the deck whose tempo the others follow. Usually BeatFreaks
picks one by itself (the "soft leader", normally the track that has been
playing longer). To choose one yourself, click the **leader** button (crown
symbol) on that deck.

**Preferences → Decks → Sync mode (Dynamic tempo tracks)** decides what
happens with tracks whose tempo changes:

- **Follow soft leader's tempo**: followers copy the leader's tempo changes.
- **Use steady tempo**: keeps the tempo steady during a transition (the older
  behaviour).

### Beat-matching by ear

You do not have to use sync. To match by ear:

1. Start the new track in your headphones.
2. Move its **tempo slider** until the beats stop drifting apart.
3. Use the **nudge** buttons (or touch the jog wheel side on a controller) to
   push it slightly forward or back until the kicks land together.

## 5.5 The tempo slider (pitch fader)

- Moving the slider changes the speed. The BPM display changes with it.
- **Preferences → Decks → Slider range** sets how far it goes: **4%**, **6%
  (semitone)**, **8% (Technics SL-1210)**, **10%**, **16%**, **24%**, **50%**
  or **90%**. A smaller range gives finer control.
- **Down increases speed**: tick this to make the slider work like a
  turntable or CDJ.
- **Speed/Tempo adjustment buttons**: set the step size for **Coarse** and
  **Fine** permanent changes and the **Temporary** (nudge) changes, and the
  **Pitch bend behavior** (**Abrupt jump** or **Smooth ramping**, with a
  **Ramping sensitivity**).
- **Reset on track load**: choose whether the speed and/or key reset to normal
  when you load a new track.

Double-click (or right-click, depending on skin) the slider to reset it to 0%.

## 5.6 Key lock

When you speed a track up, its pitch rises (it sounds higher, like a
chipmunk). **Key lock** keeps the original pitch while you change the tempo.

- Click the **key lock** button (lock symbol) on the deck to turn it on or off.
- Turning key lock on or off while the track plays may cause a tiny audio
  glitch. Set it before you start the track.
- The sound quality of key lock depends on the engine you picked in **Sound
  Hardware** (chapter 2).

**Preferences → Decks**:

- **Keylock mode**: **Original key** keeps the track's own key. **Current
  key** keeps whatever key it is in when you turn key lock on.
- **Keyunlock mode**: **Reset key** or **Keep key** when you turn key lock off.

### Changing the key on purpose

Each deck has **key up** and **key down** buttons (one semitone each), a
**sync key** button (match the other deck's key so they sound good together)
and a **reset key** button. Hover them for details. This is useful for harmonic
mixing: playing tracks whose keys fit together (chapter 10 explains the key
wheel).

## 5.7 Gain and auto level (ReplayGain)

Tracks are mastered at different loudness. Two tools help:

- **ReplayGain** (automatic): during analysis BeatFreaks measures each track's
  loudness and adjusts it so tracks play at a similar level. Set it in
  **Preferences → Normalization**: tick **Apply ReplayGain**, set the
  **Target loudness** (in LUFS, a loudness unit; the reference is -18 LUFS),
  and set the **Initial boost** for tracks not analysed
  yet. Under **Analysis** choose **ReplayGain 2.0** (recommended),
  **ReplayGain 1.0** or **Disabled**. If tracks clip, lower the target; if
  everything is too quiet, raise it.
- **Gain knob** (by hand): fine-tune each deck's level. Watch the channel
  level meter: it should touch the top (yellow) on the loudest parts but not
  stay red.

## 5.8 EQ (high, mid, low)

Each mixer channel has three EQ knobs:

- **Low**: bass and kick drum.
- **Mid**: vocals, snares, most instruments.
- **High**: hi-hats, cymbals, sparkle.

Turn left to cut, right to boost. Double-click to reset.

A classic blend:

1. Start the new track with its **Low** turned fully down.
2. Bring its fader up slowly.
3. At the start of a new phrase (usually every 8 or 16 bars), swap the bass:
   turn the old track's **Low** down and the new track's **Low** up at the
   same moment. This is called a **bass swap**.
4. Fade out the old track.

**Preferences → Mixer → Deck Equalizers** sets:

- **Equalizer Plugin**: the EQ design (several isolator and filter types; the
  default is fine).
- **Quick Effect**: the default effect for the quick effect knob.
- **Use the same EQ filter for all decks**, **Only allow EQ knobs to control
  EQ-specific effects**.
- **Bypass EQ effect processing**: turns EQs off to save processor power on
  slow PCs.
- **Reset equalizers on track load** and **Reset gain on track load**.
- **Equalizer frequency Shelves**: where **High EQ** and **Low EQ** start.
- **Main EQ**: an extra EQ on the whole main output.

You can also turn on **Kill** buttons in the skin settings (chapter 3).

## 5.9 Quick effect knob (filter)

Below the EQ is the **quick effect** knob. By default it is a **filter**:

- Turn **left**: low-pass (the track gets muffled, only bass stays).
- Turn **right**: high-pass (the bass goes away, only highs stay).
- **Middle**: off.

You can pick a different quick effect per deck (chapter 6).

## 5.10 Channel faders and crossfader

- **Channel fader**: each deck's volume.
- **Crossfader**: slides between the left side and the right side.

Each deck has a **crossfader orientation** switch: **left**, **centre**
(not affected by the crossfader) or **right**. Decks 1 and 3 are usually left,
2 and 4 right.

**Preferences → Mixer → Crossfader Preferences**:

- **Crossfader Curve**: choose **Slow fade/Fast cut (additive)** or
  **Constant power**, then drag the slider between **Mixing** (smooth, for
  long blends) and **Scratching** (sharp: a tiny move fully opens the
  channel). You can also pick **Linear** or **Logarithmic**.
- **Reverse crossfader (Hamster Style)**: swaps left and right.

On the S4 MK3 the hardware crossfader-curve switch sets these for you
(chapter 7).

![Screenshot: Preferences, Mixer page with the crossfader curve settings](images/prefs-mixer-crossfader.png)

## 5.11 Headphone cue (PFL)

**PFL** ("pre-fader listen") lets you hear a deck in your headphones before
the crowd hears it.

1. Click the deck's **headphone** button. It lights up.
2. Turn up **Headphone volume** (in the mixer middle).
3. Use **Headphone mix** to blend between what you cue (left) and the main
   mix (right).
4. **Split cue**: when on, your cue plays in one ear and the main mix in the
   other.

![Screenshot: mixer with a deck's headphone (PFL) button lit and the headphone volume and mix knobs highlighted](images/mixer-pfl-headphones.png)

Headphones need their own output. Set it in **Sound Hardware** (chapter 2).
On the S4 MK3, the headphone volume and cue mix knobs on the controller work
inside the controller's own sound card.

## 5.12 Main output and the level meters

- Keep the **main** meter out of the red. Red means **clipping** (distortion).
- If the main is too quiet, raise the main volume, not every channel gain.
- The **Booth** output (if set up) has its own volume.

## 5.13 A full first mix, step by step

1. Load track A on deck 1 and press **Play**. Fader up, crossfader left or
   centre.
2. Load track B on deck 2. Do not play yet.
3. Turn deck 2's **Low** fully down. Turn on deck 2's **headphone** button.
4. Press **SYNC** on deck 2 (tap) so its tempo matches deck 1.
5. Find the first beat of track B. Set the cue there.
6. On the first beat of a new phrase in track A, press **Play** on deck 2.
7. Listen in your headphones. If beats drift, nudge.
8. Slowly bring deck 2's fader up (or move the crossfader to the middle).
9. On the next phrase, bass swap: deck 1 **Low** down, deck 2 **Low** up.
10. Fade deck 1 out (fader down or crossfader right).
11. Turn off deck 2's headphone button. Load the next track on deck 1.

Next: [Loops, cues and effects](06-loops-cues-effects.md)
