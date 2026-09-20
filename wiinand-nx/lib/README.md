# wiinand-nx/lib (planned)

One implementation of the Wii's NAND formats, for the runtime, the launcher and
the tools - replacing the copies in `wiicompiled-nx/runtime/include/nand_*.h` and
in the Python scripts.

Shape: a small C++17 library with no dependencies beyond the standard library,
building for Switch (devkitA64) and for the host, so the same code serves the
console and the command line. Its own demo, like the other nx-mod libraries.

Planned contents, in the order they are needed:

| Piece | Why first |
|---|---|
| SYSCONF read/write | **done** - `wiinand/sysconf.h`, checked byte for byte against the runtime's own writer |
| `setting.txt` | Region and console identity, already generated per game |
| Mii database (RFL_DB) | **done** - `wiinand/miidb.h`: read, add, remove, checksum |
| Save data | Back up, restore, import from Dolphin or a real Wii |
| Title metadata (TMD, ticket) | Installing a downloaded title into the NAND |
| U8 archives | Channels pack their files this way |
| ASH decompression | The Wii Menu needs it |

`example-wii-nx/scripts/` holds working Python versions of U8, TMD/ticket and the
downloader, which are the reference for the pieces not written yet.

## Building

```sh
cmake -S . -B build && cmake --build build && ctest --test-dir build
```

On the host that also builds the tests, which check the library against files
written by the runtime. For Switch, add it to a devkitA64 build like any other
static library (`add_subdirectory`, then link `wiinand::wiinand`).
