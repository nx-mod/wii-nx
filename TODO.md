# TODO - wii-nx

- [ ] The launcher: it lists games, WiiWare and system titles and starts them
      (libdol-nx `src/launcher`). Still to do: tiles with each title's own
      banner, rather than a list of names
- [x] The Mii Channel boots, formats its database, makes and saves Miis
      (2026-10-15). Open: heads (below), speed, and starting it from the NAND
- [ ] The Wii Menu: it builds but does not boot yet (2026-10-31); then ASH
      decompression and launching titles from it
- [ ] Bomberman Blast: runs (2026-10-31), but cannot create its save data, and
      is very slow in game
- [ ] Retranslate every title for libdol-nx 4816d5b (native register contracts
      by each title's own addresses) and the removed content natives: Wii
      Sports, Wii Party, Mega Man 10, Excitebike, both Crystal Defenders - each
      after the per-title native check against a running title
- [ ] `exi.Ecb` found by `wiinx-find-globals`: six titles lack it (Wii Sports,
      Wii Party, Mega Man 10, Excitebike, both Crystal Defenders), and without
      it an EXI lock is never released (Bomberman hung on its ROM font)
- [ ] Reset that restarts the title wherever it was started from: after
      `OSResetSystem` it starts again only from the homebrew menu
- [ ] Homebrew, built and included - the only thing here that can be handed out
      ready to run: translated from each release DOL, on a `homebrew/` shelf the
      launcher lists, in order - devkitPro/wii-examples, SaveGame Manager GX,
      modmii/YAWM-ModMii-Edition, SuperFromND/rfl_mii_extractor, WiiXplorer,
      dborth/wiimc, dborth/smw-wii; the GX emulators as stress tests only
- [ ] Riivolution for every title, independent of the NAND: libdol-nx already
      replaces a disc's files and redirects saves from Riivolution XMLs
      (src/platform/fs/riivolution.cpp). Still to do: one shared
      `sdmc:/wii-nx/riivolution/` that every title reads, laid out as on a Wii's
      SD; the launcher listing a title's patches and options before it starts
      (writing `riivolution/config/<ID>.xml`); `<memory>` data patches at run
      time; code patches (Gecko, injected loaders such as Retro Rewind's)
      applied by `wiinx-build` to the DOL/REL before translating, one NRO per
      patch set; macros
- [ ] A second game playable end to end: Wii Party (`games/wiiparty-nx`),
      with its PC port's GX brought to libdol-nx's native deko3d renderer
- [ ] Mario Kart Wii and Bomberman Blast at speed on the native renderer:
      both very slow in game; Mario Kart Wii's races about 12 fps
      (libdol-nx docs/performance.md, "What would move it next"). Profile the
      host first; then a flat guest memory on Switch: one 1 GB window with
      `addr & 0x3FFFFFFF` (no mirrors to alias), mapped as haloce-nx maps its
      memory (svcMapMemory in the stack region, or svcMapProcessCodeMemory with
      hbloader's process handle)
- [ ] Miis in every title - no heads in the Mii Channel or Mario Kart Wii, the
      face texture loses its eyes and mouth - and Mario Kart Wii's race minimap
      (libdol-nx docs/deko3d.md, "Where it stands"). The Mii Channel is the
      best test: nearly everything it draws is a Mii. Not the matrix index (masked to six bits, 2026-10-31: no change)
- [ ] The Mii Channel's exit hangs (2026-10-31): after `VISetBlack(1)` the game
      thread polls a global in its own `0x800AD6F4` (OSGetCurrentThread, then
      OSDisableInterrupts/OSLockMutex in a loop) that nothing sets; audio and
      retraces go on. Not yet known whether the native-only build caused it
- [ ] The settings overlay drawn by deko3d (it is off on the native renderer)
- [x] Mega Man 9 plays (2026-10-14). Open: rows of pixels in its text, and
      a per-game button layout for its Classic Controller reads
- [ ] More WiiWare set up in `wads/` - the ones that are ordinary Wii games
      under the wrapper, rather than the Virtual Console titles that are not
- [ ] Wii Remote motion, which Wii Sports needs
- [ ] Install and uninstall titles from the NAND on the console itself: the
      user picks a WAD (or a title) and it is installed whole - ticket, TMD,
      contents, shared contents into `shared1/content.map` - or removed whole,
      as a console's Data Management does; no hand-copying files on a PC
- [ ] A NAND manager app: one place to build and look after the shared NAND
      (`sdmc:/wii-nx/system/nand`) that the Wii Menu, channels, discs and WADs
      all use - install a title or WAD, list what is installed, edit SYSCONF,
      check a title has everything it needs (its TMD's contents, `setting.txt`,
      the IOS it asks for), clean up leftover scratch files. It wraps what
      libdol-nx's `wiinx-fetch-nand`, `wiinx-install-title` and `wiinx-sysconf`
      already do; the NAND layer itself is libwii-nx's `src/nand` (its TODO lists
      the gaps)
- [ ] IOS reloads: a title that switches IOS (`IOS_ReloadIOS`,
      `ES_LaunchTitle` - some games, and the Wii Menu for every channel it
      starts) gets the new IOS's version and memory values from libdol-nx's
      table (src/platform/system/ios_versions.h), not the one it was built with
- [ ] IOS behaviour by version: where real IOSes differ (which ES calls exist,
      IOS58's USB 2.0 for homebrew, IPC details), libwii-nx answers as the
      title's own IOS would, the way Dolphin's IOS emulation does. No patched
      IOS (cIOS, Trucha) is needed: signatures are never checked here
- [ ] Wii Menu hacks from Priiloader (DacoTaco/priiloader, GPL-2.0,
      `hacks_hash.ini`): applied to the menu's program before it is translated,
      keyed by menu version - Region Free EVERYTHING, Skip / Auto-Press A at the
      Health Screen, Block Disc / Online Updates, the 480p fix, Remove NoCopy,
      Move Disc Channel. (Its Wiimmfi hack patches games as they boot, so it
      cannot reach translated code: games get Wiimmfi below.)
- [ ] Wiimmfi for disc titles: Wiimm's `wit --wiimmfi` (wiimms-iso-tools
      `patch.c`, GPL-2.0) over the extracted `main.dol` and every REL - URL and
      domain rewrites, its per-game tables, its security fix - picked by
      `wiinx-build` when `recomp.yml` says `wiimmfi: true` on a disc title. WAD
      titles already get KcrPL's WiiWare-Patcher (libdol-nx `wiinx-wiimmfi`)
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
- [ ] A whole-NAND install, the way Dolphin's "Perform Online System Update"
      makes one: every title a console of the region ships with, in one go,
      rather than the parts we have so far. And every download option a user
      may want beside it: region, a single title, IOS only, channels only,
      the hidden 00010008 titles, an update to a newer System Menu. (After the
      Wii Menu's ISFS start-up fix is confirmed on the console.)
- [x] The hidden system channels (`00010008`): EULA and region select (EUR)
      installed by hand on 2026-10-08
- [ ] A downloads page per release
- [x] One copy of the toolkit: `example-wii-nx` is gone, and libdol-nx's tools
      are what everything here calls
- [x] One copy of each game: `games/`, rather than a folder here and a
      repository of its own that drifts from it
- [x] No build of its own: libdol-nx builds a game, and the libraries are
      checked out beside it rather than pinned here
- [x] Native only: no Dawn, no Aurora build; Aurora's headers only (2026-10-31)
