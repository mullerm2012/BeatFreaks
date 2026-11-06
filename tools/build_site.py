#!/usr/bin/env python3
"""Builds the BeatFreaks website into website/dist/.

- website/src/pages/*.html are page bodies; their first line is
  <!-- title: ... | nav: ... --> and they get the shared header and footer.
- manual/*.md become website/dist/manual/*.html with a table of contents.
  Image placeholders ![Screenshot: ...](images/x.png) are shown as a
  "picture coming" box until the real file exists in manual/images/.

Usage: python3 tools/build_site.py
"""
from __future__ import annotations

import html
import pathlib
import re
import shutil

import markdown

ROOT = pathlib.Path(__file__).resolve().parent.parent
SRC = ROOT / "website" / "src"
DIST = ROOT / "website" / "dist"
MANUAL = ROOT / "manual"

NAV = [
    ("features", "features.html", "Features", True),
    ("controllers", "controllers.html", "Controllers", True),
    ("manual", "manual/index.html", "Manual", False),
    ("pricing", "pricing.html", "Pricing", False),
    ("faq", "faq.html", "FAQ", True),
    ("account", "account.html", "My downloads", False),
]

FONTS = (
    '<link rel="preconnect" href="https://fonts.googleapis.com">'
    '<link rel="preconnect" href="https://fonts.gstatic.com" crossorigin>'
    '<link href="https://fonts.googleapis.com/css2?family=Chakra+Petch:wght@500;700'
    '&family=IBM+Plex+Sans:wght@400;500;600&family=IBM+Plex+Mono:wght@500&display=swap" rel="stylesheet">'
)


def page(title: str, nav: str, body: str, depth: int) -> str:
    up = "../" * depth
    links = []
    for key, href, label, optional in NAV:
        cur = ' aria-current="page"' if key == nav else ""
        cls = ' class="opt"' if optional else ""
        links.append(f'<a{cls} href="{up}{href}"{cur}>{label}</a>')
    links.append(f'<a class="btn go small" data-buy="Buy" href="{up}pricing.html">Buy</a>')
    return f"""<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>{html.escape(title)}</title>
<meta name="description" content="BeatFreaks: four DJ decks and an ACID-style arranger in one Windows app. One-time price, no subscription.">
{FONTS}
<link rel="stylesheet" href="{up}assets/style.css">
</head>
<body>
<nav class="nav"><div class="wrap"><a class="brand" href="{up}index.html">BEATFREAKS</a><div class="links">{''.join(links)}</div></div></nav>
<main>
{body}
</main>
<footer><div class="wrap">
  <div class="cols">
    <div><h4>Product</h4><ul><li><a href="{up}features.html">Features</a></li><li><a href="{up}controllers.html">Controllers</a></li><li><a href="{up}pricing.html">Pricing</a></li><li><a href="{up}download.html">Download</a></li></ul></div>
    <div><h4>Help</h4><ul><li><a href="{up}manual/index.html">User manual</a></li><li><a href="{up}faq.html">FAQ</a></li><li><a href="{up}account.html">My downloads</a></li><li><a href="https://github.com/mullerm2012/BeatFreaks/issues">Report a problem</a></li></ul></div>
    <div><h4>Legal</h4><ul><li><a href="{up}terms.html">Terms of sale</a></li><li><a href="{up}refund.html">Refunds</a></li><li><a href="{up}privacy.html">Privacy</a></li><li><a href="https://github.com/mullerm2012/BeatFreaks">Source code</a></li></ul></div>
  </div>
  <div class="legal">
    <p>BeatFreaks is built on <a href="https://mixxx.org">Mixxx</a>, free DJ software by the Mixxx Development Team, and is released under the GNU GPL version 2 or later.</p>
    <p>BeatFreaks is an independent project, not affiliated with or endorsed by the Mixxx project, Native Instruments, Pioneer DJ, Numark or any streaming service named on this site. All trademarks belong to their owners.</p>
  </div>
</div></footer>
<script src="{up}assets/site-config.js"></script>
<script src="{up}assets/site.js"></script>
</body>
</html>
"""


HEADER_RE = re.compile(r"<!--\s*title:\s*(.*?)\s*\|\s*nav:\s*(.*?)\s*-->")


def build_pages() -> None:
    for src in sorted((SRC / "pages").glob("*.html")):
        text = src.read_text(encoding="utf-8")
        m = HEADER_RE.search(text.splitlines()[0])
        if not m:
            raise SystemExit(f"{src.name}: first line must be <!-- title: ... | nav: ... -->")
        body = text.split("\n", 1)[1]
        (DIST / src.name).write_text(page(m.group(1), m.group(2), body, 0), encoding="utf-8")


IMG_RE = re.compile(r"!\[(?P<alt>[^\]]*)\]\((?P<src>images/[^)\s]+)\)")


def manual_chapters() -> list[tuple[pathlib.Path, str]]:
    chapters = []
    for md in sorted(MANUAL.glob("*.md")):
        first = next((l for l in md.read_text(encoding="utf-8").splitlines() if l.startswith("# ")), md.stem)
        chapters.append((md, first.lstrip("# ").strip()))
    return chapters


def out_name(md: pathlib.Path) -> str:
    return "index.html" if md.stem in ("00-index", "index", "README") else md.stem + ".html"


def build_manual() -> int:
    if not MANUAL.is_dir():
        return 0
    out = DIST / "manual"
    out.mkdir(parents=True, exist_ok=True)
    if (MANUAL / "images").is_dir():
        shutil.copytree(MANUAL / "images", out / "images", dirs_exist_ok=True)
    chapters = manual_chapters()
    for i, (md, title) in enumerate(chapters):
        text = md.read_text(encoding="utf-8")

        def img(m: re.Match) -> str:
            alt = m.group("alt")
            if (MANUAL / m.group("src")).is_file():
                return m.group(0)
            label = re.sub(r"^Screenshot:\s*", "", alt)
            return f'\n<div class="shot">Picture coming: {html.escape(label)}</div>\n'

        text = IMG_RE.sub(img, text)
        # links between chapters: foo.md -> foo.html
        text = re.sub(r"\]\((?!https?:)([^)#\s]+)\.md(#[^)]*)?\)",
                      lambda m: f"]({'index' if m.group(1) in ('00-index', 'index') else m.group(1)}.html{m.group(2) or ''})", text)
        body_html = markdown.markdown(text, extensions=["tables", "fenced_code", "sane_lists", "toc"])
        body_html = body_html.replace("<table>", '<div class="tablewrap"><table>').replace("</table>", "</table></div>")
        toc = "".join(
            f'<li><a href="{out_name(c)}"{" aria-current=\"page\"" if c == md else ""}>{html.escape(t)}</a></li>'
            for c, t in chapters)
        prev_link = (f'<a class="btn small" href="{out_name(chapters[i-1][0])}">← {html.escape(chapters[i-1][1])}</a>'
                     if i > 0 else "<span></span>")
        next_link = (f'<a class="btn small" href="{out_name(chapters[i+1][0])}">{html.escape(chapters[i+1][1])} →</a>'
                     if i + 1 < len(chapters) else "<span></span>")
        body = (f'<div class="wrap manual"><aside class="toc"><h4>User manual</h4><ol>{toc}</ol></aside>'
                f'<article class="doc">{body_html}<div class="pager">{prev_link}{next_link}</div></article></div>')
        (out / out_name(md)).write_text(page(f"{title} · BeatFreaks manual", "manual", body, 1), encoding="utf-8")
    return len(chapters)


def main() -> None:
    if DIST.exists():
        shutil.rmtree(DIST)
    DIST.mkdir(parents=True)
    shutil.copytree(SRC / "assets", DIST / "assets")
    build_pages()
    n = build_manual()
    print(f"Built {len(list((SRC / 'pages').glob('*.html')))} pages and {n} manual chapters into {DIST}")


if __name__ == "__main__":
    main()
