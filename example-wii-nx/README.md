# example-wii-nx

The template and toolkit for getting a Wii game running on Switch. Every step
that works for one game lives here as a script, so the next game is a command
rather than a recipe.

Two uses:

- **Your own games.** `scripts/new-game your-dump.iso` creates a project beside
  this one, reading everything it needs from the disc.
- **An open-source demo.** A Wii homebrew program (libogc) goes through the whole
  pipeline in public CI, since it contains no Nintendo code: the one end-to-end
  test that can be published, and the proof the toolkit still works.

Game code and data are never in this repository. You build from your own disc.

## The pipeline

```
your disc dump            open-source homebrew
      |                            |
  extract-disc                 build-dol        (devkitPPC)
      |                            |
      +------------ main.dol ------+
                        |
                   inspect-dol      entry point, sections, SDA bases
                        |
                    new-game        recomp.yml + game.toml + README
                        |
                  resolve-symbols   SDK functions -> this game's addresses
                        |
                    translate       PowerPC -> C++
                        |
                    build-nro       C++ + wii-nx libraries -> <game>.nro
                        |
                     package        sdmc:/wii-nx/games/<game>/
                        |
             make-forwarder / deploy
```

## Scripts

| Script | What it does | State |
|---|---|---|
| `inspect-dol` | Entry point, sections, BSS, and the `_SDA_BASE_` / `_SDA2_BASE_` registers a project must declare. `--yaml` prints them ready to paste | **works** (verified against Mario Kart Wii) |
| `extract-disc` | Disc image (ISO/WBFS/RVZ/GCZ/CISO) → `disc/`, via Dolphin's `dolphin-tool`, with a free-space check | **works** |
| `new-game` | Both of the above plus the project: `game.toml`, `recomp.yml`, `README.md`, `.gitignore`, all read from the disc | **works** |
| `resolve-symbols` | Find each SDK function in a new game: from a symbol map where one exists, otherwise by matching code signatures taken from a game we already run | planned |
| `translate` | Run the translator over the project | planned |
| `build-nro` | Build against the prebuilt nxvk / dawn-nx / sqlite-nx releases | planned |
| `package` | Lay out `games/<game>/` for the SD card, with NACP and icon | planned |
| `make-forwarder` | NSP forwarder for the home menu | planned |
| `deploy` | Copy to a Switch over FTP | planned |
| `audit` | Count what is still game-specific, per game | planned |
| `build-dol` | Build the open-source example's DOL with devkitPPC | planned |

## Adding a game

```sh
scripts/new-game "/path/to/Your Game.iso" yourgame-nx
```

It prints what it read from the disc and writes a project you can inspect. From
there the remaining scripts take it to an NRO.

## Region

The disc ID's fourth character is the region: `P` Europe, `E` USA, `J` Japan,
`K` Korea. `new-game` records it, and the runtime uses it to present a matching
Wii to the game (PAL/NTSC video, console region and language) from the one
shared NAND.

## What a new game still needs by hand

The scripts cover everything mechanical. What they cannot do is the engine work a
game needs when it uses something no earlier game did: a different SDK version, an
unimplemented graphics or audio path, a quirk of its own. That work goes into
wiicompiled-nx, where every later game gets it for free. The `audit` script
measures how much of it is left.
