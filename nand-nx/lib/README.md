# nand-nx/lib (planned)

One implementation of the Wii's NAND formats, for the runtime, the launcher and
the tools - replacing the copies in `wiicompiled-nx/runtime/include/nand_*.h` and
in the Python scripts.

Shape: a small C++17 library with no dependencies beyond the standard library,
building for Switch (devkitA64) and for the host, so the same code serves the
console and the command line. Its own demo, like the other nx-mod libraries.

Planned contents, in the order they are needed:

| Piece | Why first |
|---|---|
| SYSCONF read/write | The settings every game reads; the launcher edits them |
| `setting.txt` | Region and console identity, already generated per game |
| Mii database (RFL_DB) | Import and export Miis; convert from the Switch's own |
| Save data | Back up, restore, import from Dolphin or a real Wii |
| Title metadata (TMD, ticket) | Installing a downloaded title into the NAND |
| U8 archives | Channels pack their files this way |
| ASH decompression | The Wii Menu needs it |

Until this exists, `example-wii-nx/scripts/` holds working Python versions of
SYSCONF, U8, TMD/ticket and the downloader, which are the reference for what the
library has to do.
