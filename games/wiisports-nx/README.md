# wiisports-nx

Wii Sports (RSPE01, NTSC-U, revision 1) on Nintendo Switch, built from your own
disc with [libdol-nx](https://github.com/nx-mod/libdol-nx).

No game code or data is in this repository. The disc is yours; everything here
was read from it, or from the [ogws](https://github.com/doldecomp/ogws)
decompilation's symbol map for this revision.

```sh
git clone --recursive https://github.com/nx-mod/wiisports-nx
cd wiisports-nx
../../libdol-nx/tools/wiinx-new-game "Wii Sports (USA) (Rev 1).rvz" .   # .rvz, .iso, .wbfs all work
../../libdol-nx/tools/wiinx-build . --dawn ../dawn-nx --aurora ../aurora-nx
```

## What is here

| | |
|---|---|
| `recomp.yml` | the translation project: entry point, small-data bases, where libdol-nx is |
| `game.toml` | what the disc says it is: ID, revision, region, title |
| `MAP.txt` | ogws's symbol map for this revision: 14,122 named functions (CC0) |
| `bindings.json` | which of this game's addresses are libdol-nx natives |
| `native/` | this game's own code, when it needs any - none yet |

## Why this game matters to the toolkit

It links the oldest SDK of any game ported so far - 2006-11 to 2007-06, where
Mario Kart Wii's is 2007-08 - and ogws names every function in it. That made it
the second game libdol-nx's natives could be *signed* from, rather than only
matched in, which is what lets a native be recognised across library builds.

It also settled a question no single game could: the same function, in two
revisions of this game, hashed differently until the masking learned to blank
the low halves of relocated addresses. Four instructions out of 138 in
`SelectThread` were the whole difference.

## Status

Set up, scanned, and never translated or run. Its natives bind by code and its
guest OS layout is known; the work after that is a build.
