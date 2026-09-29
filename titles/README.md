# titles

The Wii's own screens and channels, as programs of their own rather than games.
They run from the NAND every title here shares, so a Mii made in one shows up in
all of them.

| | Title ID | |
|---|---|---|
| [wiimenu-nx](wiimenu-nx) | `00000001/00000002` | the Wii Menu and its settings: translated, building |
| [miichannel-nx](miichannel-nx) | `00010002/48414341` | the Mii Channel: translated |

Next, in this order:

| | Title ID | Why |
|---|---|---|
| Photo Channel 1.1 | `00010002/48415941` | reads the SD card, which is served now |
| Photo Channel 1.0 | `00010002/48414141` | the same channel's first version |
| Internet Channel | `00010001/48414445` | Opera over our network layer; the Shop is built on it |
| Forecast Channel | `00010002/48414645`, `…48414650` | runs now, needs a WiiConnect24 server for content |
| News Channel | `00010002/48414745`, `…48414750` | the same |
| Shop Channel | `00010002/48414241` | as a homebrew shop: its own server, not Nintendo's |

Not needed: IOS, BC and MIOS (`00000001/*` other than the Wii Menu). The
runtime answers everything IOS would.

Nothing Nintendo owns is in this folder. Each title comes from your own console's
NAND, or is downloaded from Nintendo the way a console does:

```sh
../../libdol-nx/tools/wiinx-fetch-title 0001000248414341 miichannel
```

A title's files go in its `title/` folder, which git ignores.
