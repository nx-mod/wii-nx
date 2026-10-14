# bombermanblast-nx

`unknown`, a WiiWare or Virtual Console title, from your own copy.

## Getting the executable out

```sh
../../../libdol-nx/tools/wiinx-wad "<your title>.wad" game/
```

That writes `game/main.dol` - the content the title boots, which for these is
usually a small loader - and `game/payload.dol`, which is
what this project translates.

## Building

```sh
../../../libdol-nx/tools/wiinx-build .
```

## What is known about it

| | |
|---|---|
| entry point | `0x80006124` |
| functions found | 12014, of which 3070 are named by the symbol set |
| natives bound | 296, by signature |
| libraries | nw4r.ef, nw4r.g3d, nw4r.snd, rvl.ai, rvl.ax, rvl.cnt, rvl.dsp, rvl.dvd, rvl.dwc, rvl.exi, rvl.gx, rvl.hbm, rvl.kpad, rvl.nand, rvl.ncd, rvl.nhttp, rvl.nwc24, rvl.os, rvl.pad, rvl.rfl, rvl.sc, rvl.si, rvl.so, rvl.socket, rvl.ssl, rvl.vi, rvl.wpad |

Nothing of the title is in this folder: `game/` holds what came out of your WAD
and is not committed.

## Wiimmfi

`game/` carries the Wiimmfi patch (WiiWare Patcher's `wiiwarepatcher`, run on
content 8, the compressed game code, before `payload.dol` was expanded from
it): the online servers moved to wiimmfi.de and its security fix to the DWC
code. The originals are kept beside them as `*.original.*`. `recomp.yml`'s
hash is the patched executable's; no function or binding moved.

The OS layout (`native/bombermanblast_game.cpp`) is from `wiinx-scan
os-globals game/payload.dol` (2026-10-14).
