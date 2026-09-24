# games

One folder per game. Each holds what its disc told us and how to prepare it -
never the game itself, which you build from your own dump.

| Game | ID | Engine located | Notes |
|---|---|---|---|
| [mkwii-nx](mkwii-nx) | `RMCP01` | - | **Playable.** The game the engine was written against, so its addresses are the reference every other game is matched to |
| [nsmbwii-nx](nsmbwii-nx) | `SMNP01` | 342/574 (59.6%) | Second game; shares Mario Kart's middleware. One executable, no modules, and its own symbol map published per version |
| [spmwii-nx](spmwii-nx) | `R8PE01` | 403/574 (70.2%) | Its own engine on a plain SDK |
| [powii-nx](powii-nx) | `R7PE01` | 386/574 (67.2%) | Small, exclusive, never ported |
| [wiisports-nx](wiisports-nx) | `RSPE01` | - | Symbols and nw4r source from the ogws decompilation |
| [wiisportspack-nx](wiisportspack-nx) | `SP2E01` | 321/574 (55.9%) | Wii Sports + Resort on one disc; needs Wii Remote motion |
| [metroidprime3-nx](metroidprime3-nx) | `RM3E01` | 394/574 (68.6%) | The engine Aurora was written for |
| [pikmin2-nx](pikmin2-nx) | `R92E01` | 396/574 (69.0%) | Full public decompilation exists |

Mario Kart Wii is the game running today. Its folder here holds its project, its
disc and its translated code - copied, not moved: the working build still comes
from wiicompiled-nx until a build from here produces the same NRO.

The percentages are of *our* native set rather than of the game: 574 was how
many functions the engine replaced when that audit ran. The set is larger now,
so each game's real coverage is higher than the number says.

## Adding a game

```sh
../../libdol-nx/tools/wiinx-new-game "/path/to/Your Game.iso" games/yourgame-nx
```

It reads the disc and writes the whole project: `game.toml`, `recomp.yml`, a
README and a `.gitignore`. Screening a candidate first costs about a minute:

```sh
../example-wii-nx/scripts/extract-dol "/path/to/game.iso" /tmp/game.dol
../example-wii-nx/scripts/inspect-dol /tmp/game.dol
../example-wii-nx/scripts/resolve-symbols match <signatures.json> /tmp/game.dol
```

## What "engine located" means

How many of the engine's 574 native replacements were found in that game by
matching their code, rather than being tied to Mario Kart's addresses. It is the
cheapest predictor of how much work a game needs; the rest is a named list to
resolve by hand. For scale, the source game itself scores 97.7%.

## Status

All projects are ready and bound: each folder's `bindings.json` holds the engine
replacements found in that game, and its `recomp.yml` points the build at it.
Mario Kart Wii aside, none are translated yet - that is the next step for each,
and `../example-wii-nx/scripts/manual-adds <game>` lists what stays manual.
