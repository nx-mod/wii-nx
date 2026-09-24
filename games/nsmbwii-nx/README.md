# nsmbwii-nx

New Super Mario Bros. Wii (SMNP01, PAL, revision 2) on Nintendo Switch,
built from your own disc with [libdol-nx](https://github.com/nx-mod/libdol-nx).

No game code or data is in this repository. The disc is yours; everything here
was read from it by libdol-nx's tools, with nothing typed by hand.

```sh
git clone --recursive https://github.com/nx-mod/nsmbwii-nx
cd nsmbwii-nx
../../libdol-nx/tools/wiinx-new-game "New Super Mario Bros. Wii.iso" .
../../libdol-nx/tools/wiinx-build . --dawn ../dawn-nx --aurora ../aurora-nx
```

## What is here

| | |
|---|---|
| `recomp.yml` | the translation project: entry point, small-data bases, where libdol-nx is |
| `game.toml` | what the disc says it is: ID, revision, region, title |
| `bindings.json` | which of this game's addresses are libdol-nx natives |
| `scripts/fetch-symbols` | fetches this version's symbol map (RootCubed/NSMBW-Maps), which is not kept here |
| `native/` | this game's own code, when it needs any - empty so far |


## Its symbols

The New Super Mario Bros. Wii modding community recovered a symbol map for
every retail version ([RootCubed/NSMBW-Maps](https://github.com/RootCubed/NSMBW-Maps));
`symbols_E2_dolphin.map` is this disc's. `scripts/fetch-symbols` downloads it
and writes `MAP.txt`. It is not committed here: those maps carry no license, so
they are fetched rather than redistributed.

It named every address libdol-nx had already bound in this game by matching
code - 97 of 97, from a project with no connection to this one.

## Status

This game is the second one, and it is how libdol-nx is being made to work for
any game rather than for the first one. It translates - 17,812 functions - and
has never been run.

What it needs, in the order it will need it:

- **Its natives bind to nothing.** The scan reads all 18 library builds the
  game links (2009 ones, where Mario Kart Wii's are 2007-08), and matches none
  of them: signatures exist only for the builds the first game linked. Until
  they do, this game runs its own translated SDK code, including the OS, which
  is where threading is taken over - a boot problem, not only a slow one.
- **Its OS globals are unknown.** Where this game's copy of the Wii's OS keeps
  the run queue and the scheduler's flags has to be found before its threads
  run; the runtime says so at startup. See libdol-nx `docs/game-hooks.md`.
- **Its game objects are in RELs.** Bases, enemies, bosses and profiles are
  four compressed modules the game loads at run time
  (`disc/files/rels/*.rel.LZ`); the translator handles one configured REL, and
  these four are not translated at all.

## What was read from the disc

| | |
|---|---|
| Game ID | `SMNP01` |
| Revision | 2 |
| Region | PAL (E) |
| main.dol SHA-256 | `0373b4dbffd7c9da9d27eb7257a7677877a3e33ceb81818bab1daa93560b9907` |

`functions.map`, where its functions start, is found in the game's own code by
`wiinx-scan functions` and is regenerated rather than committed.
