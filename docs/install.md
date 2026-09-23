# Installing

## On the Switch

Games live under `sdmc:/wii-nx/games/`, one folder each, holding the program and
the disc it reads:

```
sdmc:/wii-nx/
├── config/                  settings shared by every game
├── system/                  the Wii itself: one NAND for all of them
└── games/mkwii-nx/
    ├── mkwii-nx.nro
    ├── config.toml
    ├── disc/                your own disc's files
    ├── cache/               compiled shaders, kept between launches
    └── logs/
```

Open the game from the homebrew menu.

## Building a game, once

A game has to be turned into a Switch program before it can run, and that
happens on a PC, once per game:

```sh
git clone --recursive https://github.com/nx-mod/wii-nx
cd wii-nx

wiigames-nx/mkwii-nx/scripts/extract "/path/to/your/disc.iso"
example-wii-nx/scripts/translate mkwii-nx

cmake -S . -B build -G Ninja \
      -DCMAKE_TOOLCHAIN_FILE=$DEVKITPRO/cmake/Switch.cmake \
      -DWIINX_GAME=mkwii-nx
cmake --build build
```

The result is `mkwii-nx.nro`. Copy it and the extracted disc to the card as
above.

## Adding a game that is not listed

```sh
example-wii-nx/scripts/new-game "/path/to/Your Game.iso" yourgame-nx
```

That reads the disc and writes the whole project. Screening a candidate first
takes about a minute and says how much work it needs — see
[example-wii-nx](../example-wii-nx).

## Saves

Saves live in the NAND under `sdmc:/wii-nx/system/`, shared by every game, the
way a Wii keeps them. An older install under `sdmc:/WiiCompiled/` is moved there
on first launch, saves included.
