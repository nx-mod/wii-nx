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

So getting to the code needs two things we do not have yet:

1. **ASH decompression.** Nintendo's own format, used through the Wii Menu.
   Open implementations exist.
2. **Working out which blob is the program**, then treating it as the executable.
   `example-wii-nx/scripts/unpack-u8` opens the archives; the loader in
   `0000009e` is the thread to pull.

Until then this project cannot start, which is why the Mii Channel is first: its
executable is a plain content.

## What it would need from the runtime

Beyond the shared work (binding replacements by name, booting a NAND title):

- **The title list**: what the Wii Menu shows is the NAND's installed titles. In
  wii-nx that means presenting our games as titles.
- **Banners**: each game's animated banner is its own small program the menu runs.
  That is a second executable per game, and a real test of the engine.
- **Launching**: picking a game would have to hand over to that game's build.

## Status

Not started. Blocked on ASH decompression and on finding the executable.
