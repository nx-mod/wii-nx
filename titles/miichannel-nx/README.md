# miichannel-nx

The Wii's Mii Channel, running natively on Switch. Not a game: the tool that
creates and edits Miis, which every Wii game then uses.

Why it belongs here: wii-nx keeps one shared Wii NAND, so Miis made here appear
in Mario Kart Wii and every other game, exactly as on a real console.

It is also the first title that does not come from a disc, which is what makes it
a useful early target: it proves the pipeline works for anything in the Wii's
system memory, including the Wii Menu's own settings later on.

## Get it

```sh
scripts/fetch
```

That downloads the Mii Channel from Nintendo's update servers - the same ones a
real Wii uses, and the same ones Dolphin's "Perform Online System Update" uses -
unpacks its executable to `title/main.dol` and prints its details. Takes a few
minutes.

Nothing Nintendo owns is in this repository. You download your own copy, and
`title/` is never committed.

## What the title looks like

Seven contents. The one the metadata marks as "boot" (index 6, 296 KB) is a
small loader stub; the Mii Channel itself is content index 1 (2.6 MB), which is
what `fetch-title` picks:

| | |
|---|---|
| Title ID | `0001000248414341` |
| Entry point | `0x80006124` |
| `_SDA_BASE_` (r13) | `0x803C4520` |
| `_SDA2_BASE_` (r2) | `0x803C6040` |

## Status

Translated (72 shards, about a minute), not yet built or booted.

- 311 natives bound, 57 SDK variables named in `globals.json`, the scheduler
  layout in `native/miichannel_game.cpp`.
- ES reports its own title id (`project.title_id`), so saves and `data/` land
  in `/title/00010002/48414341/`.
- Installed into the shared NAND (`/system/`) with `wiinx-install-title`.

What remains is booting from the NAND: the runtime starts from a disc today.

## Related

- The launcher (planned) will offer this alongside games, plus Mii import and
  export, so Miis can also come from a Dolphin NAND, a real Wii, or the Switch's
  own Mii database.

## What is known about it

| | |
|---|---|
| entry point | `0x80006124` |
| functions found | 10,054, of which 2,322 are named by the symbol set |
| natives bound | 311, by signature - more than any game here, because a system channel is almost all SDK and middleware |
| libraries | RVL SDK 2007-08, nw4r, HBM (the Home Button menu), RFL (Miis) |

`title/` holds what you downloaded from Nintendo and is not committed.
