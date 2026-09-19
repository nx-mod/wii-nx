# wii-nx

Wii games running natively on Nintendo Switch — statically recompiled from your own discs, not emulated.

## Games

| Game | Status |
|---|---|
| Mario Kart Wii (PAL) | Boots, menus and races playable; not full speed yet |
| New Super Mario Bros. Wii | Bring-up |

## Components

| Repo | Role |
|---|---|
| [wiicompiled-nx](https://github.com/nx-mod/wiicompiled-nx) | Engine: PowerPC → C++ translator, Wii runtime, Switch platform, game projects |
| [aurora-nx](https://github.com/nx-mod/aurora-nx) | GameCube/Wii SDK layer: GX graphics on WebGPU, Switch backend |
| [dawn-nx](https://github.com/nx-mod/dawn-nx) | WebGPU on Switch (Horizon) |
| [nxvk](https://github.com/nx-mod/nxvk) | Vulkan driver for the Switch GPU |
| [sqlite-nx](https://github.com/nx-mod/sqlite-nx) | SQLite with a native Switch VFS (shader caches) |

## Getting started

```sh
git clone --recursive https://github.com/nx-mod/wii-nx
```

Every game follows the same path: dump your disc, extract it, translate it, build, copy to your Switch.
Mario Kart Wii's steps are in the [wiicompiled-nx README](https://github.com/nx-mod/wiicompiled-nx#install);
each game's guide moves to `wiicompiled-nx/projects/<game>/` as the multi-game engine lands.

No game code or data is included — you need your own disc.

## License

GPL-3.0. Mario Kart Wii and New Super Mario Bros. Wii are © Nintendo.
