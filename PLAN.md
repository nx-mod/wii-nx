# wii-nx plan

Where things stand, how the pieces fit, and what is next. What changed and when is in
[STATUS.md](STATUS.md); the itemised list is [TODO.md](TODO.md).

## Status (2026-10-31)

| Title | Kind | State |
|---|---|---|
| Mario Kart Wii (`RMCP01`) | disc | runs: menus, flyover, races; very slow in a race; Miis and the race minimap broken |
| Mega Man 9 (`WR9E`) | WiiWare | plays well; some visual glitches remain |
| Bomberman Blast (`WM8E`) | WiiWare | runs; very slow in game; cannot create its save data |
| Mii Channel (`HACA`) | system title | runs pretty well; Mii heads missing |
| Wii Menu 4.3E | system title | builds; does not boot yet |
| Wii Party (`SUPP01`) | disc | set up as game 2 |

These five are the test suite: every library change is checked against them before it ships.

## How it fits

- **[libdol-nx](https://github.com/nx-mod/libdol-nx)** is the toolkit: the translator (.NET 8), the
  runtime, the GX renderer on deko3d, the SDK natives, the tools (`wiinx-build`, `wiinx-wad`,
  `wiinx-title-program`, `wiinx-rso`) and the launcher, `wii-nx-launcher.nro`, which lists what is
  installed under `sdmc:/wii-nx` and starts it.
- **[libwii-nx](https://github.com/nx-mod/libwii-nx)** is the Wii's own: IOS, the NAND, the Wii Remote.
- **wii-nx** (here) holds one project folder per title in `games/`, `wads/` and `titles/` - never the
  title itself - plus the NAND layout every title shares.
- **Native only.** Graphics are libdol's deko3d renderer. Aurora is read for its headers alone;
  Aurora's renderer, Dawn, NVK and the desktop build are commented out in libdol-nx's CMake.
- **Bind by signature.** A title's natives are matched by code signature into its own
  `bindings.json`; Mario Kart Wii's are its reference addresses. A symbol map is optional: a scanned
  `functions.map` works.
- Everything builds on the phone (Termux, Debian proot, aarch64): one build at a time, `-j2`.

## Next, in order

1. **Speed in game** (Mario Kart Wii, Bomberman Blast): profile the host first, then the levers in
   libdol-nx [docs/performance.md](https://github.com/nx-mod/libdol-nx/blob/main/docs/performance.md) -
   a flat guest memory window (`addr & 0x3FFFFFFF`, mapped as haloce-nx maps its memory), the GX
   FIFO decoded off the game thread, PGO and LTO, native hot routines.
2. **Miis** in every title: no heads in the Mii Channel or Mario Kart Wii, the face texture losing its
   eyes and mouth. The Mii Channel is the test: nearly everything it draws is a Mii.
3. **Bomberman Blast's save data**: it cannot create it.
4. **The Wii Menu booting**, then ASH decompression and launching titles from it.
5. **Mega Man 9's glitches**: rows of pixels in its text.
6. Wii Party as the second disc game, end to end.
7. **Homebrew**, translated from each release DOL as a WAD is (libdol-nx docs/homebrew.md, path 2), on a
   `homebrew/` shelf the launcher lists: devkitPro's wii-examples first (each libogc feature alone), then
   SaveGame Manager GX and YAWM ModMii Edition (both on the shared NAND), rfl_mii_extractor, WiiXplorer,
   WiiMC and Super Mario War. The GX emulators (Snes9x GX and its kin) only as stress tests.
8. **Riivolution** for every title from one shared folder, options chosen in the launcher ([TODO.md](TODO.md)).

## Open questions

- 60 fps at stock clocks is the target; whether it is reachable depends on what the host profile says
  about game code against runtime overhead.
- How much of the translator's speed depends on a full symbol map rather than scanned function starts.

## Rules

- Every commit in every nx-mod repo is authored and committed as nx-mod, with no trailers.
- No game code, assets or data in any repo or release: players build from their own dump.
- Public claims stay honest: "runs" or "plays", never "full speed", until it is.
