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

## Name and region

What the launcher shows for a project - "Wii Menu (Europe)" - is written into
its NRO at build time from `recomp.yml`: `title` (or `display_name`), and the
region from the fourth character of `game_id`, as every catalogue reads a disc
id: E USA, P Europe, J Japan, K Korea, A all regions. So `game_id` must be the
title's real id - a disc's from its boot.bin, a WAD's or channel's from the low
four characters of its title id (`title_id`, from its TMD) - never `unknown`,
and `region` must agree with it. libdol-nx reconfigures when `recomp.yml`
changes, so a corrected id reaches the NRO on the next build.
