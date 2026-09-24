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

## Where its executable lives (open question)

Unlike a game, the Wii Menu does not ship a plain `main.dol`. What the download
gives is seven contents:

| Content | Size | What it is |
|---|---|---|
| `00000009` | 4.5 MB | U8 archive: one LZ77-compressed library |
| `0000000b` | 686 KB | U8 archive: `RFL_Res.dat`, the Mii resource data |
| `00000049` | 1.1 MB | U8 archive |
| `0000004a` | 3.6 MB | U8 archive |
| `0000009c` | 64 B | small |
| `0000009d` | 6.2 MB | U8 archive: fonts, icons, several ASH0-compressed |
| `0000009e` | 3.8 MB | a small loader plus a 3.8 MB blob; ASH0 data inside |

Of the two things that stood in the way, one is done:

1. **ASH decompression** - now in libdol-nx's `format/archive`, and checked
   against this title's own files: `corrupt_icon.ash`, `font/font_kr.ash`,
   `layout/chn/homeBtn1.ash` and the rest expand, each into a U8 archive.
2. **Working out which blob is the program**, then treating it as the
   executable. Content `0000009e` is a loader with 3.8 MB behind it, and that
   is the thread to pull.

So the remaining question is the second one. The Mii Channel is still first,
because its executable is a plain content and needs none of this.

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

Not started. Blocked on ASH decompression and on finding the executable, so the
Mii Channel goes first.
