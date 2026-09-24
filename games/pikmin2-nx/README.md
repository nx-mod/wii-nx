# pikmin2-nx

Pikmin 2 (`R92E01`, NTSC-U, the Wii "New Play Control!" release) running natively
on Switch.

Useful for two reasons: it has a full public decompilation, so its code is better
understood than anything else we run, and it is a GameCube-era engine moved to
Wii - a different shape again from the three games we started with.

Bring your own disc. Nothing here contains game code or data.

## Get it ready

```sh
scripts/extract "/path/to/Pikmin 2 (USA) (En,Fr,Es).iso" --dol
scripts/extract "/path/to/Pikmin 2 (USA) (En,Fr,Es).iso"
```

## What came from the disc

| | |
|---|---|
| Game ID | `R92E01` (NTSC-U) |
| Entry point | `0x80006124` |
| `_SDA_BASE_` (r13) | `0x80672800` |
| `_SDA2_BASE_` (r2) | `0x806754E0` |
| Engine located automatically | **396/574 (69.0%)** |

## Status

Staged: the project holds everything its disc told us, and the engine has been
matched against it. Not translated yet.

The engine binds its replacements to this game's own addresses now, so what
remains is the translation itself, and then whatever this game does that the
engine has not met before. `../../example-wii-nx/scripts/manual-adds .` lists
what is not automatic.
