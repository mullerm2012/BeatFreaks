# Collaboration log

Append new entries at the bottom. Never edit or delete someone else's entry.
Format: see [`README.md`](README.md).

---

## 2026-11-06 05:15 UTC — Claude Code

**Did:**
- Set up this repo: Mixxx source snapshot (mixxxdj/mixxx@bc49961) plus the
  BeatFreaks commits on `main`.
- BeatFreaks core in `src/beatfreaks/`: automation lanes, recorder (Touch,
  Latch, punch-in, armed channels, loop/seek-safe), player, `.bfproj`
  project file, Hardware Test model for S4 MK3 + F1, Send-log report, log
  categories `beatfreaks.*`.
- CI: `BeatFreaks core tests` (fast, ASan/UBSan) and `BeatFreaks Windows
  installer` (Mixxx's `build.yml` trimmed to Windows x64). Removed Mixxx's
  other workflows.
- Plan (features, roadmap, checklist):
  https://claude.ai/artifact/ReVS2FPEbinKF4BVB4FAFt
- Screen designs incl. the edgy brand board:
  https://claude.ai/artifact/7bnZSkmtDpCRtUuVDyFrhJ

**Tested:** 36 core tests pass locally and on GitHub Actions. The Windows
installer build is running now (first run, 2026-11-06 05:08 UTC); not yet
known whether it succeeds.

**Next:** watch and fix the Windows build (0.2), then the Mixxx bridge (0.3).

**For ChatGPT:** welcome! Please read `CLAUDE.md`, this folder's
`README.md` and `BOARD.md`. Good tasks to claim: 1.3 (skin look and
colours), 3.2–3.4 (OBS and Stream Deck plans), or a review of
`src/beatfreaks/automation/` (fresh eyes on the recorder rules are welcome).
Tell me which you take so we don't collide.

**For mullerm2012:** once the Windows build is green, the installer is under
Actions → BeatFreaks Windows installer → latest run → Artifacts. Which
Stream Deck model do you have?
