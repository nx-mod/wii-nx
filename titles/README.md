# titles

The Wii's own screens, as programs of their own rather than games.

| | |
|---|---|
| [miichannel-nx](miichannel-nx) | the Mii Channel: the first title worth running, because its executable is a plain content |
| [wiimenu-nx](wiimenu-nx) | the Wii Menu and its settings |

They run from the NAND every game here shares, so a Mii made in one shows up in
all of them. Neither runs yet: a title has to be launchable from the NAND first,
and the Wii Menu also wants ASH decompression.

Nothing Nintendo owns is in this folder. Each user downloads their own copy of a
title from Nintendo, exactly as a console does:

```sh
../../libdol-nx/tools/wiinx-fetch-title 0001000248414641 miichannel
```
