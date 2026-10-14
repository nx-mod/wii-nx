# megaman9-nx

Mega Man 9 (`WR9E`), the WiiWare title, from your own copy.

## Getting the executable out

```sh
../../../libdol-nx/tools/wiinx-wad "Mega Man 9.wad" game/
```

That writes `game/main.dol` - the 312 KB loader the title boots, which several
WiiWare titles share - and `game/payload.dol`, which is the game: 1.4 MB of LZ11
in the WAD, 2.87 MB once expanded. It is the second one this project translates.

## Building

```sh
../../../libdol-nx/tools/wiinx-build .
```

## What is known about it

| | |
|---|---|
| entry point | `0x8000403C` |
| functions found | 10,060, of which 2,861 are named by the symbol set |
| natives bound | 311 by signature, 14 by name (2026-10-14) |
| libraries | RVL SDK Aug 2007 and Feb 2008, nw4r.snd May 2008, and the WiiWare ones: CNT, DWC, ECDK |

## State (2026-10-14)

Plays: start-up in about 3 s, the save check, menus, the stage, 57-59 fps.

Open:
- text and small sprites lose a pixel row here and there (the 256x240 picture
  the game draws, copies and scales up; not found yet)
- a sprite glitch around the character at times
- it reads the Classic Controller our remote reports, so its own Classic
  layout decides jump and shoot

What it took, all in the libraries:
- `/dev/es` opened for real (`ESP_InitLib` was a stub; every content read failed)
- `NANDInit` once, not per call (the IOS heap was set up twice)
- interrupts restored per thread on a fiber switch (a thread resumed with them
  off and its wait loop never let the NAND callback in)
- the translator: a native's register contract follows the native to this
  game's address (MM9's nw4r reader at 0x801A65F8 was treated as Mario Kart
  Wii's `__OSSetInterruptHandler` and lost its third argument)
- frames presented at the flip, not at each display copy (it copies twice a frame)
- `GXSetLineWidth`/`GXSetPointSize` applied (it draws its picture from lines)
- retraces resynced when late, not replayed

## Build notes

- **2026-10-14, native GX:** see State above. It never booted under Aurora.
- **Bindings** come from `wiinx-scan scan game/payload.dol --out bindings.json`,
  merged into the file that is there: two entries no signature can place are
  kept by hand (`GX__SetViewport`, harmless natively). Its SDK build (Jan-Mar
  2008) is one no symbol map covers, so `memcpy`, `memset`, `PSMTXConcat` and
  the rest of that build's helpers run translated.
- **Globals:** `wiinx-find-globals . --out globals.json` finds 63 of them.
- **OS layout** (`native/megaman9_game.cpp`) from `wiinx-scan os-globals
  game/payload.dol`, with the interrupt table, alarm queue and `OSLoadContext`.
- **Build** with the Dawn and Aurora trees named, where several are checked out:
  `wiinx-build . --dawn ../../../dawn --aurora ../../../wiicompiled/aurora-main`.
- **On the card** it lives in `sdmc:/wii-nx/wads/megaman9-nx/`, and reads its
  contents from the NAND (`title/00010001/57523945`).

Nothing of the game is in this folder: `game/` holds what came out of your WAD
and is not committed.
