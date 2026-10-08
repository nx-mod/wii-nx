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
- [ ] Region-free Wii Menu: a runtime option (not a patch to the menu's code)
      that lets the menu list and start titles of any region, as the
      well-known System Menu mods do
- [ ] TV format from the console's own settings: `VIGetTvFormat` answers NTSC,
      so the menu called a PAL console "NTSC" - follow setting.txt's VIDEO and
      SYSCONF (PAL, PAL60, NTSC)
- [ ] A complete NAND setup script, the first piece of the NAND manager: one
      command that builds a working NAND for the Wii Menu, channels and WADs -
      fetch the system titles a console ships with (wiinx-fetch-nand, including
      the hidden 00010008 ones: EULA HAK*, region select HAL*), install them
      against the NAND's own shared1/content.map (wiinx-install-title), take
      what only the console has from its BootMii backup (wiinx-nand-dump:
      setting.txt, SYSCONF, saves), and check the result: every TMD's contents
      present, no empty files where the menu keeps its own (iplsave.bin,
      play_rec.dat). Done by hand on 2026-10-08 for the EUR menu.
- [x] The hidden system channels (`00010008`): EULA and region select (EUR)
      installed by hand on 2026-10-08
- [ ] A downloads page per release
- [x] One copy of the toolkit: `example-wii-nx` is gone, and libdol-nx's tools
      are what everything here calls
- [x] One copy of each game: `games/`, rather than a folder here and a
      repository of its own that drifts from it
- [x] No build of its own: libdol-nx builds a game, and the libraries are
      checked out beside it rather than pinned here
