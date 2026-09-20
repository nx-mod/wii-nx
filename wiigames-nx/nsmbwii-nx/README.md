# nsmbwii-nx

New Super Mario Bros. Wii (`SMNE01`, NTSC-U, revision 2) running natively on
Switch.

The second game, and the first test of the multi-game work: it shares the EGG and
NW4R middleware with Mario Kart Wii, so it is the check that the shared layers
really are shared. It is also NTSC, which is what made the console region follow
the disc rather than the build.

Bring your own disc. Nothing here contains game code or data.

## Get it ready

```sh
scripts/extract "/path/to/New Super Mario Bros. Wii (USA) (Rev 2).iso" --dol
scripts/extract "/path/to/New Super Mario Bros. Wii (USA) (Rev 2).iso"
```

## What came from the disc

| | |
|---|---|
| Game ID | `SMNE01` (NTSC-U, revision 2) |
| Entry point | `0x80004050` |
| `_SDA_BASE_` (r13) | `0x8042F680` |
| `_SDA2_BASE_` (r2) | `0x80433080` |

## How much of the engine already fits

`example-wii-nx/scripts/resolve-symbols` locates **306 of 470 (65.1%)** of the
engine's native replacements here by matching their code - the lowest of the
titles tried, so this game is the one that will show where signature matching
needs work.

## Status

Project only: not translated yet. Waiting on the engine change that binds
replacements by name instead of Mario Kart Wii's addresses.
