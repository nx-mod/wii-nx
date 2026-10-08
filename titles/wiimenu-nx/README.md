# wiimenu-nx

The Wii Menu itself, running natively on Switch.

The long-term showpiece: the console's own home screen, listing games and
channels, with its settings, its Miis and its sound. In wii-nx it would list the
games in `sdmc:/wii-nx/games/` and launch them, which is what the standalone
launcher does in plain terms - except this is the real thing.

## Get it

```sh
scripts/fetch
```

Downloads the System Menu from Nintendo's update servers, the same ones a real
Wii uses. Nothing Nintendo owns is committed; `title/` is ignored by git.

## Where its executable lives

Unlike a game, the Wii Menu does not ship a plain `main.dol`. The download gives
nine contents, and the one it boots - `0000009b` - is a DOL whose only code
section is a kilobyte of BAT setup that maps memory and returns. The program is
the 3.5 MB "data" section behind it:

| | |
|---|---|
| version | 514, which is 4.3E: its archives carry `layout/ned` and `layout/spa` |
| loaded at | `0x81330000`, high in MEM1 |
| entry | `0x81330000`, its first instruction |
| image ends | `0x8169A4C8`, which is the blob's own first word |
| bss | `0x8169A4C8` + 64 KB |

Nothing is compressed and nothing is hidden: the blob is plain PowerPC behind an
eight word header whose first word says where the image stops.
`tools/wiinx-title-program` reads that and writes an ordinary DOL, which
`recomp.yml` translates. The small-data bases fall out of the code the same way
they do for a game (`r13` `0x8169FEE0`, `r2` `0x8169E2A0`), which is the check
that the image really is what it looks like.

The region is not incidental. The System Menu is one title whose versions are
numbered by region - 512 Japan, 513 USA, 514 Europe, 518 Korea - so asking the
servers for it without naming one gets Korea's, which numbers highest.
`wiinx-fetch-nand --region` names it.

The other contents are U8 archives of fonts, icons and layouts, several
ASH0-compressed; libdol-nx's `format/archive` expands those.

One of them is code. Content `0x09` holds `wwwlib-rvl.lz7`, the menu's web
engine (9.4 MB, LZ77): an RSO module the menu loads at `0x80080420` and links
to itself by name through `main.sel` (content `0x9A`). Its first call stopped
the menu, since only the program had been translated. `modules.json` says
where it goes, and `tools/wiinx-rso build` (run by every translate) links it
from the contents, merges it into `title/program.modules.dol` (the input
`recomp.yml` names) and adds its 27,708 function starts to `functions.map`.
The load address is the menu's heap at that point; it matched a memory dump
of the real menu, but if a later build of it loads the module elsewhere, the
menu's calls will miss.

## What it would need from the runtime

Beyond the shared work (binding replacements by name, booting a NAND title):

- **The title list**: what the Wii Menu shows is the NAND's installed titles. In
  wii-nx that means presenting our games as titles.
- **Banners**: each game's animated banner is its own small program the menu runs.
  That is a second executable per game, and a real test of the engine.
- **Launching**: picking a game would have to hand over to that game's build.

## Two stages

### 1. The shell, settings included

Boot the System Menu far enough to draw its home screen - the channel grid, the
clock, the disc slot, the pointer, its sounds - and with it the settings screens,
since "Wii Options" is part of this same binary rather than a separate title.

Needs: ASH decompression; the executable located; booting a NAND title; and a
**title list**, because what the menu shows is the NAND's installed titles, so
wii-nx has to present the games in `sdmc:/wii-nx/games/` as titles.

Settings then need only one thing beyond that: their writes have to land in the
shared NAND. The runtime already generates SYSCONF and `setting.txt` and follows
the disc's region; the menu edits them, and every game reads the result, because
they all share one NAND. The data management screens (saves and Miis) live here
too, doing in the Wii's own interface what the launcher does plainly.

### 2. Launching

Picking a game should run it. On a real console the menu asks the system software
to launch a title; here it means handing over to that game's build
(`sdmc:/wii-nx/games/<game>/<game>.nro`), which libnx can do directly.

Needs the title list from stage 1, plus each game's **banner**: a small program
of its own that the menu runs to animate the channel tile. That is a second
executable per game and the most interesting test the engine would get.

## Status

Translated (about two and a half minutes), not yet built or run.

- 282 natives bound, 55 SDK variables named in `globals.json`, the scheduler
  layout in `native/wiimenu_game.cpp`; 2,234 of its 16,089 functions named.
- ES reports `0000000100000002` (`project.title_id`).
- Installed into the shared NAND with the Mii Channel, beside your console's
  SYSCONF and Mii database.
- The translator reports four call targets with no body. Two, `0x8146B00C` and
  `0x8146CFB8`, are inside the image and untranslated. The other two,
  `0x817D0978` and `0x817D09A8`, lie past its bss, so the menu is probably
  calling into code it loads itself.

What stands between here and a home screen is the runtime work above: booting a
NAND title, and a title list to show.
