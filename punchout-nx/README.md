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
predictor of how much work it needs. For comparison: Mario Kart Wii 97.7% (itself),
Mii Channel 85.5%, Super Paper Mario 80.9%, New Super Mario Bros. Wii 65.1%.

## Status

Folder only: no disc processed yet.
