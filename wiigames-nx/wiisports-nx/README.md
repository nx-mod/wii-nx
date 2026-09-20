# wiisports-nx

Wii Sports and Wii Sports Resort running natively on Switch.

The most recognisable Wii software there is, and never ported (Switch Sports is a
different game). It is also the first title here that *needs* Wii Remote motion,
which nothing else we run exercises - the reason to do it, and the work it
brings.

Bring your own disc. Nothing here contains game code or data.

## Get it ready

```sh
scripts/extract "/path/to/Wii Sports + Wii Sports Resort.iso" --dol
scripts/extract "/path/to/Wii Sports + Wii Sports Resort.iso"
```

## What came from the disc

| | |
|---|---|
| Game ID | `SP2E01` (NTSC-U, revision 0) |
| Entry point | `0x80004050` |
| `_SDA_BASE_` (r13) | `0x801FBAC0` |
| `_SDA2_BASE_` (r2) | `0x801FCEC0` |

**321 of 574 (55.9%)** of the engine's native replacements were located
automatically.

## This disc holds two games

`main.dol` is only 1.3 MB, which is small for a Wii game: on this two-in-one
disc it is the launcher, and each game is a separate executable in the disc's
files. So this project needs one more step than the others - picking which
executable to translate - and it may end up as two projects sharing one disc.

The single-game discs (`RSPE01` Wii Sports, `RZTE01` Wii Sports Resort) are
simpler if you have them.

## What it needs beyond what we run today

- **Wii Remote motion**: pointing, acceleration, and for Resort, MotionPlus.
  The runtime reports a Wii Remote to the game, but the Switch's controllers
  have to be mapped onto its motion inputs.
- Resort assumes MotionPlus is attached; Joy-Con gyroscopes are the obvious
  source.

## Status

Staged: the project holds everything its disc told us, and the engine has been
matched against it. Not translated yet.

The engine binds its replacements to this game's own addresses now, so what
remains is the translation itself, and then whatever this game does that the
engine has not met before. `../../example-wii-nx/scripts/manual-adds .` lists
what is not automatic.
