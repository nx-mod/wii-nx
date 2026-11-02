# Installing

## On the Switch

Games live under `sdmc:/wii-nx/games/`, one folder each, holding the program and
the disc it reads:

```
sdmc:/wii-nx/
├── config/                  settings shared by every game
├── system/                  the Wii itself: one NAND for all of them
├── sd.img                   the Wii's SD card (optional)
└── games/mkwii-nx/
    ├── mkwii-nx.nro
    ├── config.toml
    ├── disc/                your own disc's files
    ├── cache/               compiled shaders, kept between launches
    └── logs/
```

Open the game from the homebrew menu.

Power off in a game or the Wii Menu closes it. Reset starts it again.

## The NAND

`system/` is the Wii's internal memory, shared by every game and channel: saves,
Miis, settings, installed channels. Copy your own console's NAND there (the
`title/`, `ticket/`, `shared1/` and `shared2/` folders), or let a game make a
fresh one.

A WiiWare or Virtual Console title installs into it from its WAD, so the Wii
Menu lists it:

```sh
../libdol-nx/tools/wiinx-install-title "<title>.wad" system/
```

## Controls

A Switch controller is a Wii Remote with a Classic Controller attached: a game
that reads the Classic Controller gets the whole pad, and one that reads the
remote gets A, B, 1 (X), 2 (Y), +, - and the D-pad.

A game played with the remote held sideways (Bomberman Blast) has
the controls turned to match: the D-pad and left stick move as you see them,
A and B are 2 and 1, X and Y are the remote's A and B. The game's project says
which way it holds the remote; `config.toml` can change it:

```toml
[controller]
wii_remote_sideways = true
```

## The SD card

The Wii's SD slot reads `sd.img`, a FAT32 image, one card for every title. With
no image there is no card. Make one on a PC:

```sh
mkfs.fat -F 32 -n WIINX -C sd.img 1048576
```

Put files on it by mounting it, and copy it to `sdmc:/wii-nx/sd.img`.

## Building a game, once

A game has to be turned into a Switch program before it can run, and that
happens on a PC, once per game:

Everything below happens beside a libdol-nx checkout, which is the toolkit and
does the building:

```sh
git clone https://github.com/nx-mod/wii-nx
git clone https://github.com/nx-mod/libdol-nx
git clone https://github.com/nx-mod/libwii-nx
cd wii-nx

../libdol-nx/tools/wiinx-build games/mkwii-nx "/path/to/your/disc.iso"
```

That reads your disc, translates the game, builds it, and leaves
`games/mkwii-nx/build/mkwii-nx.nro`. Copy it and the disc's files to the card as
above.

It needs devkitPro's Switch toolchain with deko3d, a UAM build for the shader
compiler (libdol-nx docs/deko3d.md), the .NET 8 SDK for the translator, and
aurora-wiicompiled checked out beside libdol-nx for its headers - `wiinx-build` says
what is missing and where it looked. Nothing draws through Aurora or Dawn:
graphics are libdol's own deko3d renderer, and Dawn is not needed at all.
A second machine can share the compile: `--distcc
"host:3632/4"` (libdol-nx docs/distcc.md).

## Adding a game that is not listed

```sh
../libdol-nx/tools/wiinx-new-game "/path/to/Your Game.iso" games/yourgame-nx
```

That reads the disc and writes the whole project: what the game is, where its
code starts, and how to prepare it.

`recomp.yml`'s `project` also holds what the title expects of the console,
read from the title where it can be: `ios` (its TMD's IOS), `console: ndev` for
a development console, and `remote: sideways` for a game played with the Wii
Remote held sideways.

## Saves

Saves live in the NAND under `sdmc:/wii-nx/system/`, shared by every game, the
way a Wii keeps them. An older install under `sdmc:/WiiCompiled/` is moved there
on first launch, saves included.
