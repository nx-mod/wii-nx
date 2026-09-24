# wii-nx

**Wii games, running on your Switch.**

Not emulated. Each game is translated into a Switch program ahead of time and
runs natively, using your own disc for its files.

## What you need

- A Switch running homebrew
- A dump of a game you own, on the SD card
- A PC, once per game, to build it

## Getting started

1. Copy the game's `.nro` to `sdmc:/wii-nx/games/<game>/`
2. Put your disc's files beside it
3. Open it from the homebrew menu

Building a game from your dump is one command on a PC, and it happens once per
game: see [docs/install.md](docs/install.md).

## Games

| Game | Disc | State |
|---|---|---|
| Mario Kart Wii | `RMCP01` PAL | **Playable** — boots, menus, races, saves, with sound. About a quarter of full speed |
| New Super Mario Bros. Wii | `SMNE01` NTSC-U | being translated |
| Super Paper Mario | `R8PE01` NTSC-U | set up, not yet translated |
| Punch-Out!! | `R7PE01` NTSC-U | set up, not yet translated |
| Metroid Prime 3: Corruption | `RM3E01` NTSC-U | set up, not yet translated |
| Pikmin 2 | `R92E01` NTSC-U | set up, not yet translated |
| Wii Sports | `RSPE01` NTSC-U | set up, not yet translated |
| Wii Sports + Resort | `SP2E01` NTSC-U | set up, not yet translated |

Each game is a folder in [games](games), holding what that game needs and never
the game itself.

## The Wii's own screens

The console's own titles run here too, from a NAND shared by every game — so a
Mii made once shows up everywhere. They are in [titles](titles): the Mii Channel
first, then the Wii Menu.

## Homebrew

Wii homebrew runs here, and unlike games it can be handed out already built,
because it contains nothing Nintendo made.

## Your card

```
sdmc:/wii-nx/
├── config/     settings shared by every game
├── system/     the Wii itself: one NAND for all of them
└── games/      one folder per game: the program, your disc, saves, shaders
```

## Questions

**Is this legal?** Your dump stays on your machine and nothing of Nintendo's is
distributed. You build from a disc you own.

**Will my game work?** Only the ones listed. Adding one is work, not a setting.

**Do I need a PC?** Once per game, to build it. Playing does not.

## For developers

The libraries live elsewhere, and each builds on its own:

| | |
|---|---|
| [libdol-nx](https://github.com/nx-mod/libdol-nx) | everything the Wii and the GameCube share: the CPU, GX, the DSP, the SDK, the formats, the translator and the tools |
| [libwii-nx](https://github.com/nx-mod/libwii-nx) | the Wii's own: IOS, the NAND, the Wii Remote, its settings |
| [wiicompiled-nx](https://github.com/nx-mod/wiicompiled-nx) | the engine this grew from |
| [aurora-nx](https://github.com/nx-mod/aurora-nx), [dawn-nx](https://github.com/nx-mod/dawn-nx), [nxvk](https://github.com/nx-mod/nxvk), [sqlite-nx](https://github.com/nx-mod/sqlite-nx) | graphics, and the storage under the shader cache |

This repository puts them together and holds the games.

The GameCube side of the same work is [gc-nx](https://github.com/nx-mod/gc-nx).

## License

GPL-3.0-or-later. The games and the Wii's system titles are © Nintendo; none of
them are included here.
