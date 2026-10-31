# mkwii-nx

Mario Kart Wii (RMCP01, PAL) running natively on Nintendo Switch, built from
your own disc with [libdol-nx](https://github.com/nx-mod/libdol-nx).

No game code or data is in this repository. What is here is the project that
describes the game and the few functions this game needs of its own. The disc
is yours. A symbol map is optional: `MAP.txt` can be the decompilation
community's (21,033 named functions, under no licence anyone recorded, so it is
not published here), or the function starts `wiinx-build` scans from your own
disc into `functions.map`, plus the addresses `bindings.json` binds.

```sh
cd wii-nx/games/mkwii-nx
../../../libdol-nx/tools/wiinx-build .
```

which turns `disc/` into `build/mkwii-nx.nro` for `sdmc:/wii-nx/games/mkwii-nx/`.
Supply the disc first: `../../../libdol-nx/tools/wiinx-new-game <your dump> .`

## What is here

| | |
|---|---|
| `recomp.yml` | the translation project: entry point, small-data bases, the REL, where libdol-nx is |
| `game.toml` | what the disc says it is: ID, revision, region, title |
| `MAP.txt` | the function map `recomp.yml` reads: the community's symbol map, or your scanned `functions.map` with `bindings.json`'s addresses added. Not in this repository |
| `native/` | this game's own code, compiled into its build only |


## This game's own code

libdol-nx is the Wii; anything only Mario Kart Wii does lives here.

| | |
|---|---|
| `mkwii_game.cpp` | its dynamic aspect handling, its [game hooks](https://github.com/nx-mod/libdol-nx/blob/main/docs/game-hooks.md), and where its copy of the Wii's OS keeps the scheduler's globals |
| `mkwii_dynamic_aspect_records.h` | the screens and records that aspect handling knows |
| `strap_scene.cpp` | the health-and-safety screen, replaced natively |
| `egg_gx.cpp` | EGG's `DrawGX::SetVtxState` and `LightTexture::SetupTevFinish`, for the (now commented out) Aurora build only. Until 2026-10-13 the native-GX define did not reach this folder and both ran in the native build: EGG's vertex formats never arrived, and a LightTexture quad's vertices went into the FIFO with no draw command before them |
| `mod_loader_hooks.cpp` | where mods attach |
| `product/retro_rewind_product.cpp` | what a Retro Rewind build is: its name, that it overlays the disc, and the marker its code reads |

This is also the game most of the libraries' natives were read from, so its
`bindings.json` says each one is at its own reference address - which is just
what is true of it. It is written by `tools/wiinx-reference-bindings --game .`
(libdol-nx), which keeps only addresses this game's map has a function at:
580 of 584 on 2026-10-13, the four left out read from other games or bound
inside other functions here. It binds through that table like every other game; nothing
in the build treats it differently. Any other game matches the same natives by
code signature instead.

## Retro Rewind

Set aside for now: `recomp.yml` has no `retro-rewind` profile, so only the base
game builds. The product definition stays in `native/product/`, and the
profile (pack paths, hooks, RetroWFC and Riivolution options) is in git history
for when it comes back.

libdol-nx supports mods without knowing this one: a product says whether it
overlays the disc with its own files, and this game's product definition is the
one that says yes.

## Status

Translates to 29,637 functions, byte for byte the same as the build this
project was moved out of, and builds against libdol-nx from this repository
alone.

Its compiled shaders are kept in `cache/shaders.pack` on the card, so a second
run compiles only what it has not seen.

2026-10-13, on libdol's native deko3d renderer: menus, the course flyover and
races draw; races run about 12 fps. Miis (heads, faces) and the race minimap
are broken; see wii-nx's [STATUS.md](../../STATUS.md).
