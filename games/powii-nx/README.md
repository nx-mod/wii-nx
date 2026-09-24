# powii-nx

Punch-Out!! (R7PE01, NTSC-U, revision 1) on Nintendo Switch, built from your own
disc with [libdol-nx](https://github.com/nx-mod/libdol-nx).

No game code or data is in this repository. Everything here was read from the
disc by libdol-nx's tools, with nothing typed by hand.

```sh
git clone --recursive https://github.com/nx-mod/powii-nx
cd powii-nx
../../libdol-nx/tools/wiinx-new-game "Punch-Out!!.iso" .
../../libdol-nx/tools/wiinx-build . --dawn ../dawn-nx --aurora ../aurora-nx
```

## What is here

| | |
|---|---|
| `recomp.yml` | the translation project: entry point, small-data bases, where libdol-nx is |
| `game.toml` | what the disc says it is: ID, revision, region, title |
| `bindings.json` | which of this game's addresses are libdol-nx natives |
| `native/powii_game.cpp` | where this game's copy of the Wii's OS keeps the scheduler's globals |


## Why this game

It is the third game built with libdol-nx, and the one that shows the toolkit
works on a game it was not shaped around. It links a third vintage of the SDK -
2008, where Mario Kart Wii's is 2007-08 and New Super Mario Bros. Wii's is 2009
- and everything still came out of the disc on the first run:

- **ten natives bound by code, not by address.** The SDK's hand-written
  assembly is the same in every build, so signatures taken from Mario Kart Wii
  match here: the five cache range operations, `ICInvalidateRange`, the
  interrupt trio and `OSGetTime`, each at this game's own address.
- **its OS globals, read out of its own code.** `wiinx-scan os-globals` found
  the scheduler's variables from the shape of `SelectThread`; the same scan
  returns Mario Kart Wii's known values exactly.
- **18,474 function starts**, found in the game's code, since nobody has
  published a symbol map for it.

## Status

Set up and scanned; not translated or built yet, and never run.
