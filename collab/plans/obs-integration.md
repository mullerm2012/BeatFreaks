# OBS WebSocket v5 integration plan (BeatFreaks task 3.2)

> AI-generated design proposal by DATA (ChatGPT), 2026-11-06. Review and test before implementation.
> Scope: planning only. No compiled code, working OBS connection, or hardware validation is claimed.

## Purpose and boundaries

Make BeatFreaks control **an existing OBS 28+ installation** over OBS's built-in WebSocket v5 interface (normally localhost TCP 4455), to:
1. update a deliberately restricted **live now-playing text source**;
2. switch to a user-approved **program scene**;
3. start/stop a stream with explicit confirmation and observable status.

This does **not** install OBS, alter audio routing, publish cued tracks, expose the library, or enable remote control over the public internet. Task 3.3 (the browser-source overlay) consumes the same *safe-to-broadcast projection* and is a separate deliverable. Task 3.4 (Stream Deck plugin) can use the same command layer later.

## Architecture

- `src/beatfreaks/obs/ObsClient`: non-real-time Qt WebSocket client and OBS v5 wire protocol; connect/reconnect, authentication, requests with IDs, responses, events, errors. Run entirely off the audio-engine thread; no networking, memory allocation, callbacks that may block, or OBS operations on the real-time audio thread.
- **Step 0 build gate:** run a tiny separate `OBS` default-OFF CMake option PR using `find_package(Qt6 COMPONENTS WebSockets)`, and check actual Windows vcpkg buildenv availability. If unavailable, plan a strictly loopback-only RFC 6455 text-frame adapter using existing `QTcpSocket` with compliance/security tests.
- **Protocol core:** put auth-hash, protocol op parsing, request correlation, debounce, metadata projection and sanitization in plain C++/QtCore without Qt networking under `src/beatfreaks/obs/`, included in fast `tools/beatfreaks_core` ASan/UBSan testing. Only the socket adapter depends on networking.
- `ObsController`: small typed facade: `connect`, `disconnect`, `setNowPlaying`, `selectProgramScene`, `startStream`, `stopStream`, `status`. Model state includes Disconnected, Connecting, Authenticating, Ready, Degraded, Reconnecting; display clear user-facing status.
- `BroadcastProjection`: reuse Mixxx `PlayerInfo` (`currentPlayingTrackChanged`, `getCurrentPlayingTrack()` and `getCurrentPlayingDeck()`), not a new audibility algorithm. Apply opt-in, safe mode and sanitization to the existing Live Broadcasting track-selection semantics. Single authoritative allowlist from the **audible master/output** state only. Fields: public DJ/station name (optional), title/artist of the *currently audible* track if opted in, master BPM (optional), output live status. Exclude browser/library paths, filenames, search history, cue points, hidden/private decks, headphone-only previews, unpublished tracks, authentication secrets. Never derive public metadata from deck loaded state alone.
- Preferences page: `Enable OBS integration` off by default; host default `127.0.0.1`, port `4455`, password stored using existing **QtKeychain** (`QTKEYCHAIN` / `__QTKEYCHAIN__`, already used by Mixxx Live Broadcasting) (never project files, logs, URLs, or exported profiles); source name `BeatFreaks Now Playing`; permitted scene names selected from OBS's scene list. No arbitrary script inputs from a track tag.
- Thread-safe, bounded command queue from app UI/metadata events to the OBS client; last-write-wins for now-playing (250–500 ms debounce). Rate-limit updates and retry with backoff.

## OBS v5 handshake and commands

1. Connect to `ws://127.0.0.1:4455` by default. For non-loopback use `wss://` with TLS and explicit user approval; do not expose default endpoints on an untrusted network.
2. Parse OBS `Hello` (op 0), check `rpcVersion` compatibility and auth challenge; send `Identify` (op 1). When authentication is required, compute the documented OBS challenge-response (SHA-256/base64 password+salt then challenge). Never persist or print the plaintext password or computed auth secret.
3. Wait for `Identified` (op 2) before sending `Request` (op 6) and correlate `RequestResponse` (op 7) by `requestId`. Respect `requestStatus.result/code/comment`. Parse relevant `Event` (op 5) (scene/stream state), rather than pretending an outgoing command implies success.
4. Capabilities and safe, explicit requests:
   - `GetVersion`, `GetSceneList`, `GetInputList`, `GetStreamStatus` during connect/status refresh.
   - `SetInputSettings` for a user-configured **existing** text input. Respect its kind/setting key on that platform; use a compatibility adapter rather than assuming a single text-source property works on all systems. Never create, overwrite, or change an unrelated source automatically.
   - `SetCurrentProgramScene` only if target exists and is in the explicit allowlist. If OBS Studio Mode is active, do **not** change a preview scene or fire unintended transition commands.
   - `StartStream` and `StopStream` only from a deliberate user control with a confirmation gate; check `GetStreamStatus` first. Do not trigger stream controls from metadata tags, automation playback, app startup, or network reconnection.
5. Events: `CurrentProgramSceneChanged`, `StreamStateChanged`, `InputNameChanged` / `InputRemoved` and connection closure should refresh relevant UI state. If a text source disappears, show a recoverable error instead of selecting another source silently.
6. Support request timeouts and cancellation, bounded retry with jitter, re-auth on reconnect, and no automatic replay of `StartStream`, `StopStream`, or scene-switch actions after reconnect.

## Privacy and performance gates

- Output projection uses **audibility**, not focus, loaded deck, player preview, or cue/headphone bus. During a crossfade, make now-playing policy explicit (e.g., configured main audible deck / manual override); never publish preloaded next track by accident.
- A universal `Broadcast safe mode` toggle masks track metadata instantly, including after reconnect. Example fallback: `DJ Badboy MJ — Live`. No local file paths, song source URLs, tokens, scene lists, remote addresses or passwords in diagnostic logs.
- Log only request names and OBS result codes under `beatfreaks.obs`; never payloads, metadata or credentials.
- Sanitize control characters and markup in title/artist; length-limit e.g. 200 characters; optional `[Explicit]` or user-private metadata filtering must not leak filenames by fallback.
- OBS control must not affect audio playback during disconnection or an overloaded OBS instance. All network operations asynchronous; no blocking calls on Mixxx audio callback. No silent start/stop; visible, synchronized current stream state.
- Stream connection defaults local only; password authentication required for any non-loopback host and TLS enforced. Never embed credentials in the overlay browser source.

## UI sketch

`Settings → Integrations → OBS`: host, port, password, connect/test, source picker, allowed-scene picker, Test now-playing (safe dummy text), live status. Main toolbar has an OBS connection lamp and a **two-step** Start/Stop Streaming control. Errors say how to recover (OBS → Tools → WebSocket Server Settings; verify port/password; choose an existing text source).

A performance overlay shares the BeatFreaks acid-green / pink / cyan / void-black styling but remains a separate data surface: **master-only** now-playing, BPM, stream/live badge, with no library, cue/prep widgets, or deck filenames.

## Test plan / acceptance

Unit tests with a local **fake OBS WebSocket v5 server**:
- greeting, challenge auth and no-auth, correct op/request IDs, supported RPC version, response-to-request correlation, rejection and timeout handling;
- coalesced/rate-limited text updates; source missing or wrong type; whitespace/markup in titles;
- event updates, disconnect/backoff/reconnect, source rename; no replay of streaming commands;
- loaded-but-not-audible, PFL/headphone-only, masked/private track, and crossfade edge cases never leak metadata;
- incorrect password and malformed server JSON fail closed; credentials never appear in logs.

Integration checklist, requiring real OBS 28+ and Windows:
1. Connect to OBS with WebSocket server enabled; password enabled; verify status and existing source selection.
2. DJ deck A audible while B cued: OBS only shows A; swap; verify no pre-release of B.
3. Edit title, disconnect OBS, reconnect: safe latest value appears, no unrequested stream/scene action.
4. Switch only an approved program scene; test Studio Mode on/off.
5. Start/stop stream through confirmation; check OBS state and errors; manually stop in OBS and see UI sync.
6. Run an extended session and verify no audio underrun or UI lag; check logging and file exports for secret leakage.

## Build sequencing and handoff

0. Check Qt WebSockets availability in the Windows buildenv with `OBS=OFF` default before selecting the adapter.
1. Add fake OBS server fixtures, PlayerInfo-backed `BroadcastProjection`, and fast QtCore-only tests; approve safe public metadata policy.
2. Implement v5 client and typed controller; unit-test wire protocol, failures, and reconnection.
3. Settings UI, permission/scene/source selection, explicit streaming controls.
4. Real OBS + Windows end-to-end QA; ALPHA signs off before enabling by default (default remains **off**).
5. Feed the **same** safe projection into task 3.3 browser-source overlay; publish artwork from task 1.7 separately.

**Out of scope for this plan:** streaming licensed music rights, platform upload, automatic livestream initiation, custom browser overlay implementation, universal controller mapping.

> End of AI-generated design proposal by DATA (ChatGPT); requires owner/ALPHA review.
