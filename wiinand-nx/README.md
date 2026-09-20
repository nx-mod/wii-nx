# wiinand-nx

The Wii's own system side on Switch: one shared NAND, the titles that live in it,
and the library that understands its formats.

A real Wii keeps everything but the disc in its internal memory: system settings,
Miis, save data, and the titles themselves (the Wii Menu, the Mii Channel, the
Photo Channel). wii-nx keeps one such NAND at `sdmc:/wii-nx/system/nand/`, shared
by every game, so a Mii made once shows up everywhere and saves live where a Wii
would put them.

```
wiinand-nx/
├── lib/            the formats, as one library (see below)
└── titles/
    ├── miichannel-nx/   Mii Channel - first, its executable is a plain content
    └── wiimenu-nx/      Wii Menu and its settings - blocked on ASH decompression
```

## Why a library

The same formats are needed in three places, and are currently written twice:

- **the runtime** (`wiicompiled-nx/runtime/include/nand_*.h`), which answers a
  game's requests and creates a NAND on first run;
- **the launcher** (planned), which will edit settings, import Miis and manage
  saves;
- **the scripts** (`example-wii-nx/scripts/`), in Python, for work on a PC.

`lib/` is where that moves: one C++ implementation, buildable for Switch and for
the host, with the scripts calling it rather than reimplementing it.

| Format | What it is | Where it exists today |
|---|---|---|
| SYSCONF | console settings: language, aspect, sound, sensor bar, PAL60 | runtime writes it; `scripts/sysconf` reads and edits it |
| `setting.txt` | region, model, serial - obfuscated, not encrypted | runtime writes it |
| RFL_DB | the Mii database | runtime creates an empty one |
| Save data | per title, plus the `data.bin` export format | runtime serves saves |
| TMD / ticket | title metadata and keys | `scripts/wiinand.py` |
| U8 | the archive channels pack their files in | `scripts/unpack-u8` |
| ASH | Nintendo's compression, used by the Wii Menu | **not implemented** |
| NUS | downloading titles from Nintendo | `scripts/fetch-title`, `fetch-nand` |

## Getting the titles

Nintendo still serves them, and each user downloads their own copy:

```sh
example-wii-nx/scripts/fetch-nand            # menu: pick what to download
example-wii-nx/scripts/fetch-nand --all      # system menu, 32 IOS, 9 channels
```

They land in `wii-nx/nand/`, which git ignores. Nothing Nintendo owns is in this
repository.

## What is missing to run a Wii, not just its games

1. **Boot a NAND title** instead of a disc: serve a title's contents the way the
   system software does.
2. **ASH decompression**, for the Wii Menu.
3. **A title list**: present the games in `sdmc:/wii-nx/games/` as installed
   titles, so the menu can show and launch them.
4. **Banners**: each game's channel tile is a small program of its own.

Steps 1 and 2 are what stand between us and the Mii Channel; 3 and 4 are what
turn the Wii Menu from a picture into a working home screen.
