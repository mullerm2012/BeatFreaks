# Selling BeatFreaks: plan (ALPHA, 2026-11-06)

Owner's ask (04:34): a website with everything, purchase and download,
**one-time price, no monthly fee**, discount for people who send logs,
2 downloads unless they get a new PC, keep it from spreading for free,
protect it from being broken into, a sandbox to test everything, phone use.

Not legal or tax advice. Have a lawyer read the "Licence" and "Store"
sections before launch.

## 1. Licence: what is allowed (GPL v2 or later, inherited from Mixxx)

- **Selling is allowed.** The GPL FAQ: "The right to sell copies is part of
  the definition of free software." We can charge any price.
- **Buyers must be able to get the source code** (same GPL licence), at no
  extra charge beyond the binary.
- **Buyers may share or resell their copy**, even for free. The GPL forbids
  adding restrictions, so a licence key or "2 PCs only" rule inside the app
  cannot be enforced against someone who rebuilds or patches the GPL code.
- **Our own new code is GPL too**, because it links with Mixxx.

### What we *can* control

| Control | How | Works because |
|---|---|---|
| The name and logo | Register **BeatFreaks** as a trademark; only official builds may use it | Trademark law, separate from the GPL |
| Official downloads | Store download page: 2 downloads per order, reset by email for a new PC | It's our server, not the program |
| Online services | AI song cloud, phone-remote relay, cloud backup, preset/recipe packs, future streaming integrations; need a BeatFreaks account, up to 3 devices | They run on our servers |
| Updates and support | Signed updates and help for buyers | Convenience people pay for |
| Trust | Windows code signing; only our signed installer shows "BeatFreaks" as the publisher | Copies from elsewhere aren't signed by us |

Proven example: Ardour (GPL DAW) sells binaries and keeps the source free.
Its store limits downloads (3, reset by email) and the program itself keeps
working whatever the payment status.

### Breaking in / cracking

The app is open source, so there is nothing secret to crack. What matters is:
- Accounts, payment and online services are protected server-side (standard
  web security, rate limits, no secrets in the app).
- Signed installer + checksum on the site so people can tell a real copy
  from a tampered one.

## 2. Price

Published prices in 2026: Traktor Pro 4 $149 one-time; Serato DJ Pro $299
or $11.99/month; VirtualDJ Pro about $299 one-time or $19/month.

Proposal (owner decides):
- **$149 one-time**, **$99 founder price** for launch.
- All 1.x updates included. A future 2.0 can be a one-time upgrade price;
  1.x keeps working forever.
- **Tester discount: 20%** code for anyone who sends a useful log or bug
  report from the beta (Help → Send log).
- After store fees (≈5% + $0.50 per sale), $99 → about $93.55, $149 → about $141.05.

## 3. Store

A **merchant of record** sells for us and handles sales tax/VAT worldwide,
card fraud and refunds:
- **Lemon Squeezy** (owned by Stripe): 5% + $0.50 per sale, built-in licence
  keys and file delivery with download limits. Best fit for a one-time
  desktop app.
- **Paddle**: same 5% + $0.50, more for subscriptions/enterprise.

The owner must sign up himself (identity, bank, tax form). Then we connect
the "Buy" button and the download page.

## 4. Website

Draft: `website/index.html` (preview https://claude.ai/artifact/FTCTRXdkqSuHH6h2BPAmHx).
Includes: modes, features with honest status chips, controllers, pricing,
tester discount, open-source note, download + system requirements,
Windows Sandbox tip, FAQ, Mixxx credit and non-affiliation notice.

Still needed before launch:
- [ ] Domain name (owner buys; check beatfreaks.* availability)
- [ ] Hosting (GitHub Pages is free from this repo)
- [ ] Store checkout + download links (after Lemon Squeezy sign-up)
- [ ] Terms of sale, refund policy (suggest 14 days), privacy policy
- [ ] Real screenshots and a demo video once the UI is ready
- [ ] Source-code link/offer for every release (GPL)

## 5. Sandbox / testing

- **Owner's PC:** Windows Sandbox (Windows 11 **Pro**, not Home) is a
  throwaway Windows to install the beta safely. Controllers over USB may not
  pass into the sandbox, so test hardware in normal Windows.
- **Automatic:** every change already builds the installer on a Windows
  machine on GitHub and runs the tests. Next: add an install-and-launch smoke
  test (silent MSI install, start BeatFreaks, check the log, uninstall).
- **Beta testers** with real controllers and logs (+ discount).

## 6. Phone

- First: **phone remote** as a web page served by the PC (no app store),
  board M.1.
- Later: full app for Android/iPad (Mixxx has experimental mobile builds).
  Note: Apple's App Store terms have clashed with the GPL before; an
  Android/web route is simpler. Check before investing.

## 7. Owner's to-do (only he can do these)

1. Decide the price (proposal: $99 founder / $149).
2. Sign up for Lemon Squeezy (or Paddle).
3. Buy a domain.
4. Trademark search and filing for "BeatFreaks" (USPTO).
5. Code signing: Azure Artifact Signing, $9.99/month (individuals in the
   USA/Canada). This is the one ongoing cost, for him, not his customers.
   New apps still show SmartScreen warnings until reputation builds.
6. Lawyer review of licence/store text (one-time).
