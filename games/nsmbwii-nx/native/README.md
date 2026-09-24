# native

This game's own code: the few functions only New Super Mario Bros. Wii needs,
compiled into its build alone. Everything the Wii itself does is in libdol-nx.

`nsmbwii_game.cpp` installs its guest OS layout - where
its copy of the Wii OS keeps the scheduler's globals - found by
`wiinx-scan os-globals`, not by hand. See libdol-nx `docs/game-hooks.md`.
