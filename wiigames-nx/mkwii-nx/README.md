# mkwii-nx

Mario Kart Wii (`RMCP01`, PAL) running natively on Switch. The first game, and
the one the engine was written against.

Bring your own disc. Nothing here contains game code or data.

## What is here

Its disc and its translated code, copied from the working build rather than moved:
wiicompiled-nx still produces the NRO that runs today, and stays the reference
until a build from here matches it. `native/` holds the two functions that belong
to this game rather than to the engine - its strap screen and its mod loader
hooks - which compile into this game's build alone.

| | |
|---|---|
| Game ID | `RMCP01` (PAL) |
| Entry point | `0x800060A4` |
| `_SDA_BASE_` (r13) | `0x8038CC00` |
| `_SDA2_BASE_` (r2) | `0x8038EFA0` |
| Extra module | `StaticR.rel` at `0x805102E0` |

## Why it needs no address table

The engine's ~580 native replacements name the addresses they have *in this
game*, so there is nothing to look up. Every other game supplies a table
(`resolve-symbols` + `make-bindings`) built by matching its code against this
one.

## Status

Playable: boots, menus, races, saves. Not full speed - roughly a quarter of real
time - which the performance work in PLAN.md is about. No audio yet.
