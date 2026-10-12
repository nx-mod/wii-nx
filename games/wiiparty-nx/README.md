# wiiparty-nx

Wii Party (`SUPP01`, revision 0, PAL), running natively on Switch with
[libdol-nx](https://github.com/nx-mod/libdol-nx). Game 2, after Mario Kart Wii.

The disc is never in this repository: you supply your own.

## What is here

| | |
|---|---|
| `game.toml`, `recomp.yml` | what the game is, and the translator project, both read from the disc |
| `bindings.json` | the natives `wiinx-scan` found in this game: 311, 31 of them from signatures taken from its own code (`wiinx-near-map`) |
| `functions.map` | 8,154 function starts from `wiinx-scan functions` |
| `disc/` | your extracted disc, untracked. `disc/UPDATE/` holds the disc's update partition (system menu 4.2E, Mii Channel, IOS WADs) |

## Prior art

[arelkair/wiiparty-recomp](https://github.com/arelkair/wiiparty-recomp)
(GPL-3.0, like this project) ports this game to PC. It reimplements the
hardware under the translated game: GX to D3D11/OpenGL (TEV, indirect
textures, fog, EFB copies, lit Miis), recompiled AX DSP microcode, and
Bluetooth, IOS and NAND. Wherever its way is better, we use it, credited.
Wherever ours is, we keep ours. On Switch, performance decides. Its GX
renderer is the candidate for libdol-nx's native deko3d path, and its
`analysis/dol_functions.csv` lists every function's start and size.

## Build it from your own disc

```sh
../../../libdol-nx/tools/wiinx-build .
```

## Status

2026-10-12: set up and scanned. Not translated yet.
