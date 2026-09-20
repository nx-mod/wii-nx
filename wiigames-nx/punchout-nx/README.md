# punchout-nx

Punch-Out!! (Wii) running natively on Switch.

A Wii exclusive that was never ported or re-released, and still wanted. Chosen as
a next target because it is cheap: a small disc and modest graphics, but it
stresses things the first three games do not - tight input timing, and a
presentation style built around short, precise animation.

It also plays fine with buttons, so it does not depend on the Wii Remote motion
support we have not written yet.

Bring your own disc. Nothing here contains game code or data.

## Get it ready

```sh
scripts/extract "/path/to/Punch-Out.iso" --dol   # ~5 seconds: ID, entry point, bases
scripts/extract "/path/to/Punch-Out.iso"         # the whole disc (~4.5 GB)
```

The first form prints everything `recomp.yml` needs. Fill in the `TODO`s from it
(or let `example-wii-nx/scripts/new-game` write the whole project from scratch).

## Worth checking first

```sh
../example-wii-nx/scripts/resolve-symbols match <signatures.json> disc/sys/main.dol
```

How much of the engine is found in this game by matching code is the cheapest
predictor of how much work it needs. Of 574 replacements: Super Paper Mario 70.2%,
Pikmin 2 69.0%, Metroid Prime 3 68.6%, this game 67.2%, New Super Mario Bros. Wii
59.6%, Wii Sports 55.9%.

## What came from the disc

| | |
|---|---|
| Game ID | `R7PE01` (NTSC-U, revision 1) |
| Entry point | `0x80006124` |
| `_SDA_BASE_` (r13) | `0x804178A0` |
| `_SDA2_BASE_` (r2) | `0x8041C8C0` |

**386 of 574 (67.2%)** of the engine's native replacements were located in this
game automatically; `bindings.json` holds them, and `recomp.yml` points the build
at it.

## Status

Staged: the project holds everything its disc told us, and the engine has been
matched against it. Not translated yet.

The engine binds its replacements to this game's own addresses now, so what
remains is the translation itself, and then whatever this game does that the
engine has not met before. `../../example-wii-nx/scripts/manual-adds .` lists
what is not automatic.
