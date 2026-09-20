# mkwii-nx

Mario Kart Wii (`RMCP01`, PAL) running natively on Switch. The first game, and
the one the engine was written against.

Bring your own disc. Nothing here contains game code or data.

## Where it is being built from

Still `wiicompiled-nx`, not here. The working build lives in that repository
along with its translated code, and it stays there until the wii-nx build can
produce the same NRO. This folder holds what belongs to the game rather than to
the engine, ready for that move:

| | |
|---|---|
| Game ID | `RMCP01` (PAL) |
| Entry point | `0x800060A4` |
| `_SDA_BASE_` (r13) | `0x8038CC00` |
| `_SDA2_BASE_` (r2) | `0x8038EFA0` |
| Extra module | `StaticR.rel` at `0x805102E0` |

## Why it needs no bindings file

The engine's ~580 native replacements name the addresses they have *in this
game*, so there is nothing to translate them to. Every other game supplies a
table (`resolve-symbols` + `make-bindings`); this one is the reference the tables
are built from.

## Status

Playable: boots, menus, races, saves. Not full speed - roughly a quarter of real
time - which the performance work in PLAN.md is about. No audio yet.
