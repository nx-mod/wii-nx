# titles

The Wii's own screens, as programs of their own rather than games. They run from
the NAND every title here shares, so a Mii made in one shows up in all of them.

| | Title ID |
|---|---|
| [wiimenu-nx](wiimenu-nx) | `00000001/00000002` |
| [miichannel-nx](miichannel-nx) | `00010002/48414341` |

Nothing Nintendo owns is in this folder. Each title comes from your own console's
NAND, or is downloaded from Nintendo the way a console does:

```sh
../../libdol-nx/tools/wiinx-fetch-title 0001000248414341 miichannel
```

A title's files go in its `title/` folder, which git ignores.
