# titles

The Wii's own screens, as programs of their own rather than games. They run from
the NAND every title here shares, so a Mii made in one shows up in all of them.

| | Title ID | State |
|---|---|---|
| [wiimenu-nx](wiimenu-nx) | `00000001/00000002` | builds; does not boot yet |
| [miichannel-nx](miichannel-nx) | `00010002/48414341` | runs pretty well; Mii heads are missing |

Nothing Nintendo owns is in this folder. Each title comes from your own console's
NAND, or is downloaded from Nintendo the way a console does:

```sh
../../libdol-nx/tools/wiinx-fetch-title 0001000248414341 miichannel
```

A Dolphin NAND works too (Android: Settings > User Data > Export User Data). Its
contents are already decrypted: `title/<id>/content/title.tmd` lists them, and a
content marked shared is in `shared1/`, named by the SHA-1 the TMD gives it in
`shared1/content.map` (the Wii Menu's `00000009`, which holds `wwwlib`, is one).
The Mii Channel's program is content index 1; the Wii Menu's is its boot content,
unpacked by `../../libdol-nx/tools/wiinx-title-program`, with its modules merged
by `wiinx-rso build`.

A title's files go in its `title/` folder, which git ignores.

## Name and region

What the launcher shows for a project - "Wii Menu (Europe)" - is written into
its NRO at build time from `recomp.yml`: `title` (or `display_name`), and the
region from the fourth character of `game_id`, as every catalogue reads a disc
id: E USA, P Europe, J Japan, K Korea, A all regions. So `game_id` must be the
title's real id - a disc's from its boot.bin, a WAD's or channel's from the low
four characters of its title id (`title_id`, from its TMD) - never `unknown`,
and `region` must agree with it. None of it is typed in: libdol-nx's
`wiinx-identity` reads it from the title (the disc's boot.bin, the title's TMD
in `title/` or `game/`) and corrects `recomp.yml`, and `wiinx-translate` and
`wiinx-build` run it first. libdol-nx reconfigures when `recomp.yml` changes,
so the NRO's name follows on the next build.
