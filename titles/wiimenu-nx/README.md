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
seven contents, and the one it boots - `0000009e` - is a DOL whose only code
section is a kilobyte of BAT setup that maps memory and returns. The program is
the 3.7 MB "data" section behind it:

| | |
|---|---|
| loaded at | `0x81330000`, high in MEM1 |
| entry | `0x81330000`, its first instruction |
| image ends | `0x8166CC48`, which is the blob's own first word |
| bss | `0x8166CC48` + 64 KB |
| built | `systemmenu.rvl.1005130953`, by `irduser@IPLBUIL` (content `0000009c`) |

Nothing is compressed and nothing is hidden: the blob is plain PowerPC behind an
eight word header whose first word says where the image stops.
`tools/wiinx-title-program` reads that and writes an ordinary DOL, which
`recomp.yml` translates. The small-data bases fall out of the code the same way
they do for a game (`r13` `0x81672660`, `r2` `0x81670A60`), which is the check
that the image really is what it looks like.

The other contents are U8 archives of fonts, icons and layouts, several
ASH0-compressed; libdol-nx's `format/archive` expands those.

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

The executable is located and the project translates it. `wiinx-scan` binds 272
natives in it - more than either Mega Man - and the symbol set names 2,306 of
its functions, with 8,325 more found by their prologues.

Not yet built or run. What stands between here and a home screen is the runtime
work below: booting a NAND title, and a title list to show.
