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

`example-wii-nx/scripts/resolve-symbols` locates **342 of 574 (59.6%)** of the
engine's native replacements here by matching their code - among the lowest of the
titles tried, because much of what this game calls is EAD's own engine code rather
than the SDK. `bindings.json` holds them, and `recomp.yml` points the build at it.

## Status

Being translated: the first game after Mario Kart Wii to go through the whole
path. The engine binds 342 of its replacements to this game's addresses, and
translation runs but stops partway, on a call target that is not a real function
address - the kind of thing only a second game surfaces.

The engine binds its replacements to this game's own addresses now, so what
remains is the translation itself, and then whatever this game does that the
engine has not met before. `../../example-wii-nx/scripts/manual-adds .` lists
what is not automatic.
