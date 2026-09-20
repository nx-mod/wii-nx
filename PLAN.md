# wii-nx plan

Working notes: where things stand, what was decided and why, and what is next.

## Status

| Part | State |
|---|---|
| Mario Kart Wii (PAL `RMCP01`) | Boots, menus, save data, races playable. **~4x too slow**; no audio |
| New Super Mario Bros. Wii (NTSC-U `SMNE01`, Rev 2) | Project skeleton only (`wiicompiled-nx/projects/nsmbwii`). Bring-up targets the NTSC-U disc; Mario Kart is PAL, so the two also prove the engine is not region-locked |
| Super Paper Mario (`spmwii-nx`) | Planned alongside NSMBW; the [spm-decomp](https://github.com/SeekyCt/spm-decomp) symbol maps feed bind-by-symbol |
| wiicompiled-nx | Engine + Switch platform; `switch` branch, v1.0.0 released (runtime-only zip) |
| aurora-nx | Forked, `switch` branch created; still stock upstream - the Switch backend has not moved here yet |
| dawn-nx | Horizon port: native surface, static Vulkan from NVK, platform gaps |
| nxvk | Used unmodified |
| sqlite-nx | Switch VFS complete: in-process locking, every journal mode but WAL; demo NRO passes 21/21 on hardware; CI green. Wired into Aurora: shader caches persist (2859/2860 hits on second launch) |

## Architecture

- **wii-nx** is the workspace: one clone, five submodules, one top-level build. It also removes the
  current inversion where the game compiles inside Dawn's CMake project.
- **Games are folders**, not repos: `wiicompiled-nx/projects/<game>/` holds the translator project,
  symbol map, game-only native code and that game's README.
- **Three tiers in the engine**: SDK (OS/GX/AX/DVD/NAND/VI) and shared middleware (EGG, NW4R) stay in
  the engine; only genuine game quirks go in a project folder.
- **Bind by symbol, not address.** Today 575 native replacements and 314 guest addresses are keyed to
  Mario Kart's binary. Replacements should name the SDK function they implement, with a per-game symbol
  map saying where it lives. This is what makes game #3 cheap.
- **Libraries**: only SQLite needed a Switch port (sqlite-nx). freetype/libpng/xxhash/zlib come from
  devkitPro portlibs; zstd/fmt/imgui/tracy compile clean from source. No other forks warranted.

## Performance

Measured: ~4x too slow at stock clocks; raising the GPU clock changes nothing; ~55 ms of CPU per frame
against a 16.6 ms budget. Ruled out: TLS (plain globals on Switch), direct-call dispatch, paired singles
(NEON + FMA), FP control writes. The translator already fuses compares into branches and elides leaf
spills, so the easy translator wins are taken.

Found, not yet tested: **worker threads were on the game's core**. libnx creates threads with cpuid -2
(the process default core), and Horizon does not time-slice at normal priority, so Dawn/Aurora frame
encoding, presentation, shader compilation and audio mixing only ran when the game thread blocked -
while cores 1 and 2 sat idle. Fixed in `runtime/platform_switch/switch_thread_stack.cpp`.

Next levers, in order:
1. Thread placement (done, awaiting a test run).
2. Attribution profile: splits game code vs native runtime, names the top functions on each side.
3. Dual-core graphics: `fifo::drain()` -> `process()` decodes the command stream on the game thread at
   every sync point; move it to its own thread and block only where the CPU needs GPU results.
4. PGO + LTO (devkitA64 GCC 16.1 has libgcov and the LTO plugin).
5. Native replacements for hot game routines (nw4r math, decompression, THP decode).
6. Direct guest RAM access (Switch forces the checked page-table path today).

## Found 2026-09-19

- **The guest-memory globals were reached through the GOT.** devkitPro's NintendoSwitch CMake
  platform file turns CMAKE_POSITION_INDEPENDENT_CODE on for every project, so each access the
  translated code makes to gFlatGuestBase / the page-bias tables / the write guard first loaded the
  address from the GOT, and GCC could not inline or allocate registers across calls. Measured on one
  shard: 8175 GOT loads, 1297 direct.
  - Dropping -fPIC gives 0 GOT loads but does not link: absolute addresses in read-only data cannot
    be relocated when an NRO is loaded ("read-only segment has dynamic relocations").
  - -fPIE alone changes nothing (8175 GOT loads): extern data still goes through the GOT.
  - **Marking those declarations hidden** (runtime/include/mkw_visibility.h) plus -fvisibility=hidden
    on the project's own code: 111 GOT loads, 10260 direct. Keeps PIC, so it links. Awaiting a
    hardware run for the speed figure.
- **-mcpu=native** in the translated code meant generic ARMv8 tuning (the cross compiler cannot
  detect a CPU). Now -mcpu=cortex-a57+crc+crypto.
- **Region is hard-coded to Europe** (setting.txt AREA/MODEL/VIDEO/GAME, SYSCONF). With one NAND
  shared by every game, generate the region-dependent settings at launch from the disc ID's
  region letter (P/E/J/K). Needed for NSMBW (NTSC-U).
- **SD layout** is now sdmc:/wii-nx/{config,system,games/<game>}/ (runtime/include/switch_layout.h),
  migrated automatically from sdmc:/WiiCompiled/. Launcher planned at sdmc:/switch/wii-nx/wii-nx.nro:
  finds games, per-game settings, launches, installs NSP forwarders.
- **Per-library config files** in sdmc:/wii-nx/config/ (Dawn toggles, NVK/Mesa options, Aurora).
- **add-game script**: ISO -> disc/ + main.dol -> ID, entry, SDA bases -> recomp.yml -> translate
  -> build -> games/<name>/.
- nxvk: fixed the window being left broken after a Vulkan swapchain is destroyed (stale
  preallocated buffer slots). dawn-nx: full demo NRO, 10/10 on hardware at 60 fps.

## Launcher, NAND tools and example-wii-nx (planned)

- **Launcher** `sdmc:/switch/wii-nx/wii-nx.nro` (open source, CI release): finds games under
  sdmc:/wii-nx/games/, per-game and shared settings, launches (envSetNextLoad), installs NSP
  forwarders (as sphaira does; needs sigpatches).
- **NAND tools** in the launcher: Mii manager (import/export, Dolphin NAND, .mii), Switch Mii ->
  Wii Mii (libnx Mii database), save manager (backup/restore/import), NAND import (Dolphin NAND,
  BootMii nand.bin + keys.bin), system settings.
- **System Update** from Nintendo's update servers (NUS), as Dolphin does: Wii Menu, IOS, channels
  including the Mii Channel, installed into system/nand. Decision: follow Dolphin's precedent and
  embed the Wii common key; the user's own keys.bin is an optional alternative. Nothing
  Nintendo-owned in any repo or release.
- **Mii Channel** as a recompiled title: the first title loaded from the NAND instead of a disc.
- **A Wii on the Switch**, in three stages (see wiimenu-nx/README.md):
  1. **Shell**: the Wii Menu drawing its home screen, with our games presented as installed titles.
  2. **Settings**: "Wii Options" is part of the same title; its edits write to the shared NAND that
     every game already reads.
  3. **Launching**: picking a game hands over to that game's NRO, and each game's banner is its own
     small program the menu runs.
  Blocked first on ASH decompression and on finding the System Menu's executable inside its contents
  (six U8 archives plus a loader with a 3.8 MB blob).
- **example-wii-nx**: open-source Wii homebrew (libogc) through the whole pipeline in public CI:
  DOL (devkitPPC container) -> translate -> NRO against the lib releases -> package -> NSP ->
  release. Tests the translator, core runtime and packaging, not the Nintendo SDK layer; start
  with a tiny console program (needs a small libogc layer). Its scripts are the template for
  every game (extract-disc, inspect-dol, make-project, resolve-symbols, translate, build-nro,
  package, make-forwarder, deploy, audit).
- **Distribution check** (2026-09-19): the v1.0.0 zip's dsp_coef.bin and wii_bootstrap files are
  byte-identical to Dolphin's published Data/Sys copies.
- **Native replacement audit**: 583 addresses; ~570 Nintendo SDK, ~8 EGG/NW4R, ~3 Mario Kart only.
  The code is already universal; the addresses are not. Next: register by name, resolve per game
  from symbol maps or SDK signatures.

## Work queue

| Work | Size | Confidence |
|---|---|---|
| CI building the NRO | 1 session | High |
| dawn-nx as a prebuilt artifact | 1 session | High |
| ~~sqlite-nx wired in (fixes the shader cache)~~ | done | - |
| Portlibs + build flip + umbrella CMake | 1-2 | High |
| PGO + LTO | 1 + two long builds | Medium |
| Direct guest RAM mapping | 1 | Medium |
| Native hot routines | 1-3 | Medium |
| Dual-core graphics | 3-5 | Low-medium |
| Symbol binding (575 + 314) | 2-4 | Medium |
| SDK signature database | 2-3 | Medium |
| New Super Mario Bros. Wii bring-up | 1 + unknown | Low |
| aurora-nx restructure, upstreaming | 3-5 | Medium |

Builds take 2+ hours on the phone, which is why CI comes first.

## Open questions

- aurora-nx base: current upstream + Switch backend only (WiiCompiled's Aurora delta sorted into
  drop / upstream / keep), versus carrying that delta forward.
- Whether upstream WiiCompiled wants the multi-game split; our Switch platform layer is the opening.
- 60 fps at stock clocks is the target. Whether it is reachable depends on what the attribution profile
  says about game code versus runtime overhead.

## Releases

Every library publishes prebuilt packages so a project links them instead of rebuilding: tagged
`<upstream version>-nx-mod-v<n>`, one convention across all of them.

| Library | Release |
|---|---|
| nxvk | `v26.2.2-nx-mod-v1`: Vulkan and OpenGL portlibs, 36 smoke-test NROs |
| sqlite-nx | CI green; needs a tag |
| dawn-nx | needs CI, then a tag |
| wiinand-nx | when it has more than SYSCONF and Miis |

## Rules

- Every commit in every nx-mod repo is authored and committed as nx-mod.
- No game code, assets or data in any repo or release - players build from their own dump.
- Every sample/smoke-test NRO writes its report to `sdmc:/` as well as the screen, so results can be fetched over FTP
  (sqlite-nx: `sdmc:/switch/sqlite-nx-demo.log`; dawn-nx: `sdmc:/dawn_smoke_log.txt`; nxvk: `sdmc:/<test>.log`).
- Public claims stay honest: "playable, not full speed" until it is.
