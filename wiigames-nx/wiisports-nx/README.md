# wiisports-nx

Wii Sports (`RSPE01`) running natively on Switch.

The one game here with a public decompilation behind it:
[ogws](https://github.com/doldecomp/ogws) (CC0, forked as
[ogws-nx](https://github.com/nx-mod/ogws-nx)), about 35% matched. That is not
enough to build the game from source - the rest is still recompiled like every
other title - but what it has is exactly what this engine wants: **nw4r, EGG,
the RVL SDK and RVLFaceLib**, the shared libraries every Nintendo Wii game is
built on, plus a complete symbol map for both retail revisions.

Bring your own disc. Nothing here contains game code or data.

## Get it ready

```sh
scripts/extract "/path/to/Wii Sports.iso" --dol   # ID, revision, entry, small-data bases
scripts/fetch-symbols                             # MAP.txt from ogws, for your revision
scripts/extract "/path/to/Wii Sports.iso"         # the whole disc
```

Fill the `TODO`s in `recomp.yml` and `game.toml` from the first command. ogws
supports revision 0 and revision 1 (rev 1 SHA-1
`8bb422971b88b5551a37de98db69557df7b46637`); check which yours is before
fetching the map.

## Why a symbol map matters here

Mario Kart Wii is the only other game with one. Everywhere else the engine finds
its native replacements by matching code signatures, which locates 56-70% of
them. With every function named, a replacement can bind by name directly, and a
profile reads as `nw4r::g3d::G3DState::LoadResShpPrimitive` rather than an
address. `fetch-symbols` writes 14,122 functions, 7,590 named.

## Not the same as wiisportspack-nx

[wiisportspack-nx](../wiisportspack-nx) is `SP2E01`, the bundle disc with Wii
Sports Resort. This is the original standalone release.

## Status

Staged: entry point from ogws, symbol map scripted. Waiting on a disc for the
small-data bases and checksum, then translation.
