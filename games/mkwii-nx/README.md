# mkwii-nx

Mario Kart Wii (RMCP01, PAL) running natively on Nintendo Switch, built from
your own disc with [libdol-nx](https://github.com/nx-mod/libdol-nx).

No game code or data is in this repository. What is here is the project that
describes the game, the few functions this game needs of its own, and the
symbol map the decompilation community made. The disc is yours.

```sh
git clone --recursive https://github.com/nx-mod/mkwii-nx
cd mkwii-nx
../../libdol-nx/tools/wiinx-build . --dawn ../dawn-nx --aurora ../aurora-nx
```

which turns `disc/` into `build/mkwii-nx.nro` for `sdmc:/wii-nx/games/mkwii-nx/`.
Supply the disc first: `../../libdol-nx/tools/wiinx-new-game <your dump> .`

## What is here

| | |
|---|---|
| `recomp.yml` | the translation project: entry point, small-data bases, the REL, where libdol-nx is |
| `game.toml` | what the disc says it is: ID, revision, region, title |
| `MAP.txt` | Mario Kart Wii's symbol map, from the decompilation community |
| `native/` | this game's own code, compiled into its build only |


## This game's own code

libdol-nx is the Wii; anything only Mario Kart Wii does lives here.

| | |
|---|---|
| `mkwii_game.cpp` | its dynamic aspect handling, its [game hooks](https://github.com/nx-mod/libdol-nx/blob/main/docs/game-hooks.md), and where its copy of the Wii's OS keeps the scheduler's globals |
| `mkwii_dynamic_aspect_records.h` | the screens and records that aspect handling knows |
| `strap_scene.cpp` | the health-and-safety screen, replaced natively |
| `mod_loader_hooks.cpp` | where mods attach |
| `product/retro_rewind_product.cpp` | what a Retro Rewind build is: its name, that it overlays the disc, and the marker its code reads |

This is also the game libdol-nx's natives were written from, so its
`recomp.yml` says `native_bindings: registered`: their addresses are this
game's. Any other game matches them by code signature instead.

## Retro Rewind

The pack builds as a second executable beside the base game, from the copy you
installed - none of it is in this repository. `recomp.yml`'s `retro-rewind`
profile has the paths and the hooks; point `mod_root` at your `RetroRewind6`
folder and translate with that profile.

libdol-nx supports mods without knowing this one: a product says whether it
overlays the disc with its own files, and this game's product definition is the
one that says yes.

## Status

Translates to 29,637 functions, byte for byte the same as the build this
project was moved out of, and builds against libdol-nx from this repository
alone. Its shader pipelines are in `cache/`, untracked, so the first run after
a build compiles none of them.

It runs at roughly a quarter speed on hardware as of the last measurement,
which is where the work is. Nothing built from libdol-nx has been on hardware
yet.
