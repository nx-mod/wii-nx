# nsmbwii-nx

New SUPER MARIO BROS. Wii (SMNE01, revision 2, NTSC-U), running natively on Switch with
[libdol-nx](https://github.com/nx-mod/libwii-nx).

Created by `wiinx-new-game` from a disc dump. The disc itself is never in this
repository: you supply your own.

## Status

Fresh project: translated code not built yet.

## Build it from your own disc

```sh
../../../libdol-nx/tools/wiinx-scan scan disc/sys/main.dol --out bindings.json   # where its natives are
../../../libdol-nx/tools/wiinx-translate .                                       # DOL -> C++
```

## What came from the disc

| | |
|---|---|
| Game ID | `SMNE01` |
| Revision | 2 |
| Region | NTSC-U (E) |
| main.dol SHA-256 | `0373b4dbffd7c9da9d27eb7257a7677877a3e33ceb81818bab1daa93560b9907` |

`recomp.yml` holds the entry point and small-data bases read from `main.dol`;
`../../../libdol-nx/tools/wiinx-inspect-dol disc/sys/main.dol` prints them again at any time.
