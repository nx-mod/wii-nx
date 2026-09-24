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
| natives bound | 266, by signature |
| libraries | RVL SDK Aug 2007 and Feb 2008, nw4r.snd May 2008, and the WiiWare ones: CNT, DWC, ECDK |

Nothing of the game is in this folder: `game/` holds what came out of your WAD
and is not committed.
