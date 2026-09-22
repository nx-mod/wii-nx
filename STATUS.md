# Status

2026-09-22. Measured on hardware (Switch, 1280x720 output) with Mario Kart Wii.

## Mario Kart Wii

| | |
|---|---|
| Runs | boots, menus, races, saves, music and sound |
| Menus | ~30-40 fps plain; ~15 fps on screens with movie buttons and glow |
| Races | ~13 fps: a 77 ms frame against 16.7 ms for 60 |
| Where the frame goes | the CPU: the GPU waits ~3 µs a frame for work |
| Known issues | occasional audio static, lag on animated-button screens |

Of a race frame, ~45 ms is the game's own translated code, ~13 ms is graphics
command handling on the game's core (Aurora building ~480 draws, the runtime
walking display lists), ~6 ms audio and idle work.

## What changed today

- **Threaded GX decode** (`[video] threaded_gx`, off by default): graphics
  decoding on its own core. Works through menus and races, but still waits on
  the decoder ~90 times a frame and runs slower than inline decode, with
  occasional audio static. Stays off until those syncs are gone.
- **Profiling that sees everything**: a sampler records the game thread's real
  program counter every millisecond, and the decode worker's call stack once a
  second (`logs/pcsamples.bin`), resolved offline against the ELF.
- **Faster draws**: Aurora's pipeline lookup no longer re-reads a 2.7 KB config
  per draw (~2 ms a race frame, ~3 MB of RAM back).
- **`example-wii-nx/scripts/translate`**: translates a game from its own
  folder in one command - Mario Kart Wii in about 2 minutes. Every game's
  `recomp.yml` (and `new-game`'s template) had its workspace one folder too
  high; fixed.
- **libwii-nx**, the Wii as a library: staged, with its core and the first two
  natives ported, each for every library build that needs it. About a tenth
  written; its platform layer still lives in the engine.
- **Credits**: libjpeg (IJG), and the decompilations the natives follow
  (pikmin2, ogws, mkw), are attributed in each repository.

## Commits

| Repository | Commit | |
|---|---|---|
| [wiicompiled-nx](https://github.com/nx-mod/wiicompiled-nx) | `e85c6cb` | Credit the sources the natives follow |
| | `d4ab0f3` | Threaded GX decode (experimental, off), a real CPU profiler, faster draws |
| [libwii-nx](https://github.com/nx-mod/libwii-nx) | `ea571ed` | Stage libwii-nx: core, READMEs and notices |
| | `f7b0e3b` | Port the first natives: nw4r lyt Pane::CalculateMtx and RVL SDK THP |
| wii-nx | this commit | STATUS.md, `translate`, `recomp.yml` fixes, READMEs, engine at `d4ab0f3` |

## Next

1. The ~90 remaining syncs a frame in threaded decode: find their call sites
   and move them into the command stream, then turn it on by default.
2. From the profiler: the hottest game functions, as natives in libwii-nx
   (g3d `CalcWorld` / `CalcView` are located in Mario Kart Wii).
3. libwii-nx: its build scanner, then the platform layer ported in.
