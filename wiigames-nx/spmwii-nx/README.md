# spmwii-nx

Super Paper Mario (`R8PE01`, NTSC-U, revision 2) running natively on Switch.

Intelligent Systems' own engine sitting straight on the Wii SDK, which is why it
is a useful third game: it uses little of the EGG/NW4R middleware Mario Kart Wii
and New Super Mario Bros. Wii share, so it tests that the SDK layer stands alone.

Bring your own disc. Nothing here contains game code or data.

## Get it ready

```sh
scripts/extract "/path/to/Super Paper Mario (USA) (Rev 2).iso" --dol   # quick look
scripts/extract "/path/to/Super Paper Mario (USA) (Rev 2).iso"         # the disc (~4.5 GB)
```

## What came from the disc

| | |
|---|---|
| Game ID | `R8PE01` (NTSC-U, revision 2) |
| Entry point | `0x80006124` |
| `_SDA_BASE_` (r13) | `0x80574900` |
| `_SDA2_BASE_` (r2) | `0x80575C60` |

## How much of the engine already fits

`example-wii-nx/scripts/resolve-symbols` locates the engine's native
replacements in this game by matching their code:

**380 of 470 (80.9%)** - the best of any title tried so far, better than New
Super Mario Bros. Wii's 65%, which is what "its own engine on a plain SDK" looks
like.

## Symbols

[spm-decomp](https://github.com/SeekyCt/spm-decomp) has symbol maps for this
game, but mainly for the PAL versions (plus partial NTSC-U rev 0), and it
deliberately excludes the SDK, NW4R and MSL libraries. So it can name some game
code; the SDK functions the runtime replaces come from signature matching.

## Status

Staged: the project holds everything its disc told us, and the engine has been
matched against it. Not translated yet.

The engine binds its replacements to this game's own addresses now, so what
remains is the translation itself, and then whatever this game does that the
engine has not met before. `../../example-wii-nx/scripts/manual-adds .` lists
what is not automatic.
