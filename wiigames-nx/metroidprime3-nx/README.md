# metroidprime3-nx

Metroid Prime 3: Corruption (`RM3E01`, NTSC-U) running natively on Switch.

The best engine fit we have. Aurora - our graphics layer - was written for the
Metroid Prime decompilation, so this game's way of driving the Wii's graphics
hardware is the one it was built against. A Wii exclusive that was never ported
(Prime 1 was remastered; Corruption was not).

Bring your own disc. Nothing here contains game code or data.

## Get it ready

```sh
scripts/extract "/path/to/Metroid Prime 3 - Corruption (USA).iso" --dol
scripts/extract "/path/to/Metroid Prime 3 - Corruption (USA).iso"
```

## What came from the disc

| | |
|---|---|
| Game ID | `RM3E01` (NTSC-U) |
| Entry point | `0x80006320` |
| `_SDA_BASE_` (r13) | `0x806801C0` |
| `_SDA2_BASE_` (r2) | `0x806869C0` |
| Engine located automatically | **394/574 (68.6%)** |

## What to expect

It uses the graphics hardware harder than anything we run, so it will find gaps
Mario Kart never does. It also expects Wii Remote pointing, which we have, and
its own controls are more demanding than a racing game's.

## Status

Project ready; disc not extracted and nothing translated yet.
