# TODO - wii-nx

- [ ] The launcher: it lists games, WiiWare and system titles and starts them
      (libdol-nx `src/launcher`). Still to do: tiles with each title's own
      banner, rather than a list of names
- [ ] The Mii Channel, which needs a title to be launchable from the NAND
- [ ] The Wii Menu, which needs that and ASH decompression
- [ ] Homebrew, built and included - the only thing here that can be handed out
      ready to run
- [ ] A second game playable end to end
- [ ] Mega Man 9 built: it is translated, and the build is the step left
- [ ] More WiiWare set up in `wads/` - the ones that are ordinary Wii games
      under the wrapper, rather than the Virtual Console titles that are not
- [ ] Wii Remote motion, which Wii Sports needs
- [ ] A NAND manager app: one place to build and look after the shared NAND
      (`sdmc:/wii-nx/system/nand`) that the Wii Menu, channels, discs and WADs
      all use - install a title or WAD, list what is installed, edit SYSCONF,
      check a title has everything it needs (its TMD's contents, `setting.txt`,
      the IOS it asks for), clean up leftover scratch files. It wraps what
      libdol-nx's `wiinx-fetch-nand`, `wiinx-install-title` and `wiinx-sysconf`
      already do; the NAND layer itself is libwii-nx's `src/nand` (its TODO lists
      the gaps)
- [ ] The hidden system channels (`00010008`, region select and EULA among
      them), which the Wii Menu may ask for: not on the NAND yet
- [ ] A downloads page per release
- [x] One copy of the toolkit: `example-wii-nx` is gone, and libdol-nx's tools
      are what everything here calls
- [x] One copy of each game: `games/`, rather than a folder here and a
      repository of its own that drifts from it
- [x] No build of its own: libdol-nx builds a game, and the libraries are
      checked out beside it rather than pinned here
