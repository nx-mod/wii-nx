# TODO - wii-nx

- [ ] The launcher: games and installed channels as tiles, each with its own
      banner
- [ ] The Mii Channel, which needs a title to be launchable from the NAND
- [ ] The Wii Menu, which needs that and ASH decompression
- [ ] Homebrew, built and included - the only thing here that can be handed out
      ready to run
- [ ] A second game playable end to end
- [ ] Mega Man 9 built: it is translated, and the build is the step left
- [ ] More WiiWare set up in `wads/` - the ones that are ordinary Wii games
      under the wrapper, rather than the Virtual Console titles that are not
- [ ] Wii Remote motion, which Wii Sports needs
- [ ] A downloads page per release
- [x] One copy of the toolkit: `example-wii-nx` is gone, and libdol-nx's tools
      are what everything here calls
- [x] One copy of each game: `games/`, rather than a folder here and a
      repository of its own that drifts from it
- [x] No build of its own: libdol-nx builds a game, and the libraries are
      checked out beside it rather than pinned here
