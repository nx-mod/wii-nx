# crystaldefenders-nx

Crystal Defenders R1 (`WCIE`), the WiiWare title, from your own copy.

Square Enix shipped two: R1 and R2 (`WC2E`) are separate games on the same
engine rather than a game and its revision, so what is learned here should land
on the other almost entirely.

## Getting the executable out

```sh
../../../libdol-nx/tools/wiinx-wad "Crystal Defenders R1.wad" game/
```

That writes `game/main.dol` - the 312 KB loader several WiiWare titles share -
and `game/payload.dol`, the game itself. This one is a plain executable rather
than a compressed content, so nothing has to be expanded first.

## Building

```sh
../../../libdol-nx/tools/wiinx-build .
```

## What is known about it

| | |
|---|---|
| entry point | `0x8000403C` |
| functions found | 6,596, of which 2,500 are named by the symbol set |
| natives bound | 237, by signature |
| libraries | RVL SDK Jul 2008, and the WiiWare ones: CNT, DWC |

Nothing of the game is in this folder: `game/` holds what came out of your WAD
and is not committed.
