# megaman10-nx

`WRXE`, a WiiWare or Virtual Console title, from your own copy.

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
| entry point | `0x80004050` |
| functions found | 11291, of which 1761 are named by the symbol set |
| natives bound | 261, by signature |
| libraries | nw4r.snd, rvl.ai, rvl.ax, rvl.cnt, rvl.dsp, rvl.dvd, rvl.dwc, rvl.ecdk, rvl.enc, rvl.exi, rvl.gx, rvl.hbm, rvl.kpad, rvl.nand, rvl.ncd, rvl.nhttp, rvl.nhttpcreate, rvl.nwc24, rvl.os, rvl.sc, rvl.si, rvl.so, rvl.socket, rvl.ssl, rvl.vi, rvl.wpad |

Nothing of the title is in this folder: `game/` holds what came out of your WAD
and is not committed.
