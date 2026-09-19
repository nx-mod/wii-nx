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

- **Everything was compiled -fPIC.** devkitPro's NintendoSwitch CMake platform file turns
  CMAKE_POSITION_INDEPENDENT_CODE on for every project. In one translated shard 8249 of 8349
  global accesses went through the GOT, and GCC could not inline or use interprocedural register
  allocation across functions. Off on Switch now (Dawn, Aurora, runtime); rebuild pending test.
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

## Rules

- Every commit in every nx-mod repo is authored and committed as nx-mod.
- No game code, assets or data in any repo or release - players build from their own dump.
- Every sample/smoke-test NRO writes its report to `sdmc:/` as well as the screen, so results can be fetched over FTP
  (sqlite-nx: `sdmc:/switch/sqlite-nx-demo.log`; dawn-nx: `sdmc:/dawn_smoke_log.txt`; nxvk: `sdmc:/<test>.log`).
- Public claims stay honest: "playable, not full speed" until it is.
