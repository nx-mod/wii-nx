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

It needs devkitPro's Switch toolchain, and aurora-nx, dawn-nx and nxvk checked
out beside libdol-nx - `wiinx-build` says which are missing and where it looked.

## Adding a game that is not listed

```sh
../libdol-nx/tools/wiinx-new-game "/path/to/Your Game.iso" games/yourgame-nx
```

That reads the disc and writes the whole project: what the game is, where its
code starts, and how to prepare it.

## Saves

Saves live in the NAND under `sdmc:/wii-nx/system/`, shared by every game, the
way a Wii keeps them. An older install under `sdmc:/WiiCompiled/` is moved there
on first launch, saves included.
