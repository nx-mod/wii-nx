# wii-nx

Wii games running natively on Nintendo Switch — statically recompiled from your
own discs, not emulated. Each game's PowerPC code is translated to C++ ahead of
time and compiled for the Switch, with the Wii's system software answered
natively rather than simulated.

Game code and data are never in this repository. You build from your own disc,
and the Wii's system titles are downloaded from Nintendo by each user.

## Games

| Game | Disc | State |
|---|---|---|
| Mario Kart Wii | `RMCP01` PAL | **Playable**: boots, menus, races, saves, with sound. About a quarter of full speed |
| New Super Mario Bros. Wii | `SMNE01` NTSC-U | Translating |
| Super Paper Mario | `R8PE01` NTSC-U | Staged |
| Punch-Out!! | `R7PE01` NTSC-U | Staged |
| Metroid Prime 3: Corruption | `RM3E01` NTSC-U | Staged |
| Pikmin 2 | `R92E01` NTSC-U | Staged |
| Wii Sports | `RSPE01` NTSC-U | Staged, symbols from the ogws decompilation |
| Wii Sports + Wii Sports Resort | `SP2E01` NTSC-U | Staged |

"Staged" means the project exists with everything read from the disc, and the
engine has been matched against it, but it has not been translated yet. See
[wiigames-nx](wiigames-nx) for how much of the engine each one already fits.

The Wii's own titles are in [wiinand-nx](wiinand-nx): the Mii Channel first,
then the Wii Menu.

## Layout

```
wii-nx/
├── wiigames-nx/     one folder per game
├── wiinand-nx/      the Wii's system side
│   ├── lib/         its formats: settings, Miis, saves, archives
│   └── titles/      miichannel-nx, wiimenu-nx
├── example-wii-nx/  the toolkit: discs, titles, symbol matching, project setup
└── CMakeLists.txt   one build: libraries, engine, game, NRO
```

Each library below builds on its own and can be used by any Switch project; this
repository only puts them together.

| Library | Role |
|---|---|
| [wiicompiled-nx](https://github.com/nx-mod/wiicompiled-nx) | The engine: PowerPC → C++ translator, and the Wii's system software answered natively |
| [aurora-nx](https://github.com/nx-mod/aurora-nx) | The Wii's graphics (GX) on WebGPU |
| [dawn-nx](https://github.com/nx-mod/dawn-nx) | WebGPU on Switch |
| [nxvk](https://github.com/nx-mod/nxvk) | The Vulkan driver underneath |
| [sqlite-nx](https://github.com/nx-mod/sqlite-nx) | SQLite with a Switch filesystem layer (the shader caches) |
| [libwii-nx](https://github.com/nx-mod/libwii-nx) | The Wii as a library: the console's platform layer and versioned native versions of the middleware games link. Staged; the engine still provides both today |
| `wiinand-nx/lib` | The Wii's NAND formats, shared by the engine, the tools and the launcher |

## Getting started

```sh
git clone --recursive https://github.com/nx-mod/wii-nx
cd wii-nx

# your own disc
wiigames-nx/mkwii-nx/scripts/extract "/path/to/your/disc.iso"
example-wii-nx/scripts/translate mkwii-nx

cmake -S . -B build -G Ninja \
      -DCMAKE_TOOLCHAIN_FILE=$DEVKITPRO/cmake/Switch.cmake \
      -DWIINX_GAME=mkwii-nx
cmake --build build
```

The result is `mkwii-nx.nro`. Copy it, and the extracted disc, to your SD card:

```
sdmc:/wii-nx/
├── config/                  settings shared by every game
├── system/                  the emulated Wii: one NAND for all of them
└── games/mkwii-nx/
    ├── mkwii-nx.nro
    ├── config.toml
    ├── disc/                your extracted disc
    ├── cache/               compiled shaders, kept between launches
    └── logs/
```

An older install under `sdmc:/WiiCompiled/` is moved here on first launch, saves
included.

`-DWIINX_GAME=` builds the libraries alone, which is what CI does: no game code
can live on a public runner.

## Adding a game

```sh
example-wii-nx/scripts/new-game "/path/to/Your Game.iso" yourgame-nx
```

That reads the disc and writes the whole project. Screening a candidate first
costs about a minute and predicts how much work it needs — see
[example-wii-nx](example-wii-nx).

## Where the work is

- **Speed.** Mario Kart runs at roughly a quarter of real time, on the CPU: the
  GPU waits for work. The levers are native versions of the hottest middleware
  routines (collected in libwii-nx), graphics-command decoding on its own core
  (`[video] threaded_gx`, experimental), and compiler-wide optimisation.
  `PLAN.md` has the measurements.
- **A second game running.** Everything up to translation works for any game;
  translating one end to end is the current edge.
- **Wii Remote motion**, which Wii Sports needs.

## License

GPL-3.0. The games and the Wii's system titles are © Nintendo; none of them are
included here.
