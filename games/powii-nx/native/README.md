# native

This game's own code: the few functions only Punch-Out!! needs, compiled into
its build alone. Everything the Wii itself does is in libwii-nx.

`powii_game.cpp` installs its guest OS layout - where its copy of the Wii's OS
keeps the scheduler's globals - found by `wiinx-scan os-globals` rather than by
hand. See libwii-nx `docs/game-hooks.md`.
