---
name: run-wii-nx
description: Build and drive the wii-nx pipeline - set up the workspace, translate a Wii game, disc or WAD into C++ and cross-compile it into a Switch NRO. Use when asked to run, build, test, set up, translate a game, make a project from a WAD, check globals, or reproduce a build for wii-nx.
---

wii-nx turns a Wii game into a Switch program ahead of time. Nothing here runs on
Linux: the product is an `.nro` for a Switch running homebrew. What an agent can
drive is the pipeline that makes it, through
`.claude/skills/run-wii-nx/wiinx.sh` (paths below are relative to `wii-nx/`).

## Prerequisites

Ubuntu 24.04. These were already present in the container this was verified in:

```bash
sudo apt-get install -y cmake ninja-build git python3 tcl curl dotnet-sdk-8.0
```

## Setup (once, about a minute)

```bash
.claude/skills/run-wii-nx/wiinx.sh setup
```

Everything goes beside `wii-nx/`, in the folder holding it:

- **Sibling repos**, cloned shallow on the nx-mod branches this work uses:
  libdol-nx, libwii-nx and wiicompiled-nx on `switch-testing`, libgc-nx on
  `main`, dawn-nx on `switch`.
- **Dawn's four Vulkan/SPIR-V submodules**, fetched from GitHub.
- **Abseil** at a pinned substitute commit, with two Switch patches applied.
- **`deps/`**: git clones of what Aurora would otherwise download as tarballs,
  plus a SQLite amalgamation built from source.
- **devkitPro**, if `$DEVKITPRO` (default `/opt/devkitpro`) has none: it's pulled
  from the `devkitpro/devkita64` Docker image's layers with curl, no Docker
  needed.
- **An empty nxvk stand-in**, if real nxvk isn't installed.

Check the result any time:

```bash
.claude/skills/run-wii-nx/wiinx.sh status
```

## Run (agent path)

| command | what it does | time here |
|---|---|---|
| `wiinx.sh synthetic [DIR]` | the no-disc test game: make it, translate it, build `synthetic_nro` | ~25 min cold on 4 cores, most of it Dawn |
| `wiinx.sh new-wad FILE.wad DIR` | a whole project from a WAD you own (recomp.yml, functions.map, bindings.json, globals.json, native/) | ~20 s |
| `wiinx.sh build PROJECT [--translate]` | compile a project into `PROJECT/build`, optionally translating first | minutes to hours by game size |
| `wiinx.sh globals PROJECT` | re-derive the SDK variables from the game's own code, with the evidence for each | seconds |
| `wiinx.sh tests` | libdol-nx host tests (ctest) | ~1 min warm |

`build` and `synthetic` report one of three outcomes:

- `NRO: <path>`: a finished program.
- `COMPILED; link stopped on: vk_icdGetInstanceProcAddr` (exit 3): every object
  built, but only the nxvk stand-in is installed. This is the normal result in
  this container.
- `build failed`: the first errors, plus the path to `PROJECT/build.log`.

A WAD project, end to end:

```bash
.claude/skills/run-wii-nx/wiinx.sh new-wad /path/to/Mega_Man_9.wad /tmp/mm9
.claude/skills/run-wii-nx/wiinx.sh globals /tmp/mm9 | head
.claude/skills/run-wii-nx/wiinx.sh build /tmp/mm9 --translate
```

## Run (human path)

On the Switch: copy `PROJECT/build/<name>.nro` and the game's files to
`sdmc:/wii-nx/games/<name>/` and open it from the homebrew menu. That needs real
nxvk and hardware; neither is available here.

## Gotchas

- **Tarball downloads are refused (403)**, including GitHub archive and release
  URLs, sqlite.org and chromium.googlesource.com. Git over GitHub works. That's
  why `deps/` holds clones and the build passes `FETCHCONTENT_SOURCE_DIR_*`.
  Configuring `cmake/game` by hand without those flags hangs on, then fails at,
  the first download.
- **Dawn's `.gitmodules` points at googlesource**, so `git submodule update`
  fails. The driver fetches the same commits from GitHub by SHA.
- **Dawn's pinned abseil (2d78d7c) isn't on GitHub.** The driver pins master
  commit 8c138bb and applies two `sed`s: Horizon's `pthread_t` is a pointer, and
  its `tzname` works like newlib's.
- **Don't run plain `cmake --build` on a game.** It builds every third-party
  target, including Dawn's demo and Abseil's signal handler, which don't
  compile for Horizon. Build `<game>_nro` only, as the driver does.
- **nxvk can't be fetched here**: its releases need api.github.com release
  assets. With the empty stand-in the link stops on `vk_icdGetInstanceProcAddr`
  and nothing else. Any other undefined symbol is a real bug.
- **Translated output records absolute paths.** Build a project where it was
  translated, or translate it again.
- **`new-wad` names fewer functions than a committed project.** On Mega Man 9 it
  matches `bindings.json` and `globals.json` byte for byte, but leaves four VI
  functions unnamed that `wads/megaman9-nx/functions.map` names.

## Troubleshooting

- **`fatal: unable to access 'https://chromium.googlesource.com/...': CONNECT tunnel failed, response 403`**:
  a plain `git submodule update` in dawn-nx. Use `wiinx.sh setup`.
- **`upload-pack: not our ref 2d78d7ce...`**: fetching Dawn's pinned abseil
  from GitHub. Use the substitute commit that setup pins.
- **`WARNING: [ sqlite-check-tcl ]: Found tclsh but no tclConfig.sh`** during
  setup: harmless. `make sqlite3.c` only needs `tclsh`.
