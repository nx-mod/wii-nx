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
your disc dump      a Wii system title     open-source homebrew
      |                     |                       |
  extract-disc          fetch-title            build-dol       (devkitPPC)
      |                     |                       |
      +---------------- main.dol -------------------+
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
| **Getting the code** | | |
| `extract-dol` | Pull `main.dol` out of a disc image without extracting the disc: reads the partition table, unwraps the title key and decrypts only the clusters the executable occupies. Seconds, a few MB | **works** |
| `extract-disc` | The whole disc (ISO/WBFS/RVZ/GCZ/CISO) via Dolphin's `dolphin-tool`, with a free-space check | **works** |
| `fetch-title` | Download a Wii system title from Nintendo's update servers and unpack its executable. The content a title marks as "boot" is often a loader stub, so contents are scored and the real one chosen | **works** |
| `fetch-nand` | A reference set of system titles, from a menu or by flag. Resumes | **works** |
| `unpack-u8` | List or extract a U8 archive, which is how channels pack their files, flagging ASH and LZ77 compression inside | **works** |
| **Understanding it** | | |
| `inspect-dol` | Entry point, sections, BSS, and the `_SDA_BASE_` / `_SDA2_BASE_` registers a project must declare. `--yaml` prints them ready to paste | **works** |
| `resolve-symbols` | Find a game's copies of the functions the engine replaces by matching their code, with operands that legitimately differ masked out, then following the calls inside every match | **works**: 59-77% per game, no wrong matches on a self-check |
| `audit` | How universal the engine is: its replacements sorted into SDK, middleware and game-specific | **works** (98.1% / 1.5% / 0.3%) |
| `manual-adds` | What a game still needs by hand, and `--template` to start one | **works** |
| **Building it** | | |
| `new-game` | A complete project read from a disc: `game.toml`, `recomp.yml`, `README.md`, `.gitignore` | **works** |
| `make-bindings` | Resolved addresses as the C++ table a game's build links | **works** |
| `sysconf` | Read and edit a Wii SYSCONF (the console's settings) | **works** |
| `translate` | Run the translator over a project | planned |
| `build-nro` | Build against the prebuilt library releases | planned |
| `package` | Lay out `games/<game>/` for the SD card, with icon and metadata | planned |
| `make-forwarder` | An NSP forwarder for the home menu | planned |
| `deploy` | Copy to a Switch over FTP | planned |
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

## Screening a candidate

About a minute, and it predicts how much work a game needs:

```sh
scripts/extract-dol "/path/to/game.iso" /tmp/game.dol     # seconds
scripts/inspect-dol /tmp/game.dol                          # ID, entry, bases
scripts/resolve-symbols match <signatures.json> /tmp/game.dol
```

The last number is how much of the engine was located in that game by matching
code. For scale: 97.7% against the game the signatures came from, 85.5% Mii
Channel, 80.9% Super Paper Mario, 69.0% Pikmin 2, 68.6% Metroid Prime 3, 67.2%
Punch-Out!!, 65.1% New Super Mario Bros. Wii.

## What a new game still needs by hand

The scripts cover everything mechanical. Two things they cannot do:

- **What the matcher missed.** Those functions fall back to the game's own
  translated code, which is correct but slower. `manual-adds` lists them.
- **What only that game does.** It goes in `<game>/native/`, compiled into that
  game's build alone. Mario Kart Wii is the worked example, with two functions:
  its strap screen and its mod loader hooks.

Engine work a game needs because it uses something no earlier game did - a
different SDK version, an unimplemented graphics or audio path - goes into
wiicompiled-nx, where every later game gets it for free.
