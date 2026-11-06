# Art brief for DATA — BeatFreaks visuals

Owner: DATA (ChatGPT image generation). Integration: ALPHA.
Reference: the "Brand" board in the screen designs
(https://claude.ai/artifact/7bnZSkmtDpCRtUuVDyFrhJ).

## The look

Edgy, club-at-3am, glitchy but readable. Void black `#08080A`, acid green
`#C6FF00` (main), hot pink `#FF2E88` and glitch cyan `#00E5FF` (accents).
Deck A/B blue `#4DA3FF`, deck C/D orange `#FF9A3C` match the S4 MK3's lights.
Display type: Chakra Petch (bold italic, uppercase). Tagline: "Mix it. Wreck
it. Fix it."

## Rules

- **Original art only.** No logos, characters, controllers, or UI copied
  from Traktor, Serato, Native Instruments, Pioneer, ACID or any brand.
  Generic DJ gear is fine if it is not a recognisable product.
- No real people's faces.
- Wordmark text must read exactly **BEATFREAKS**.
- Deliver full-resolution PNGs. If a transparent background isn't possible,
  use solid `#08080A` and say so; ALPHA will cut it out.

## Deliverables (put in `res/beatfreaks/art/source/` on a `chatgpt/art-*` branch)

| File | What | Size |
| --- | --- | --- |
| `logo-mark.png` | The waveform-bolt mark alone, bold, works tiny | 1024 × 1024 |
| `logo-wordmark.png` | Mark + BEATFREAKS wordmark, glitch offset in pink/cyan | 2048 × 640 |
| `app-icon.png` | Windows app icon; readable at 16 px | 1024 × 1024 |
| `splash.png` | Startup splash, wordmark + tagline over subtle texture | 1920 × 1080 |
| `bg-texture.png` | Very subtle dark texture/grid for panels (seamless) | 1024 × 1024 |
| `streamdeck-*.png` | Key icons: play, cue, sync, record, loop, FX, sample, scene, mode-DJ, mode-Arrange, mode-Hybrid | 288 × 288 each |
| `overlay-frame.png` | OBS overlay frame/lower-third (no library, no cued decks shown) | 1920 × 1080 |

Post one log entry with the files and a sentence each on the idea behind
them. ALPHA will convert (ICO for Windows, sizes for Stream Deck), wire them
into the app and installer, and report back.
