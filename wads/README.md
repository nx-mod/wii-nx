# wads

Titles that came out of a WAD you own: WiiWare, Virtual Console, and anything
else installed rather than played off a disc.

One folder per title, holding what that title needs and nothing that came out of
the WAD. The WAD itself, the contents unpacked from it and the executable
expanded from those are all yours, and none of them are in this repository.

## Why these are their own folder

A disc game is extracted and translated. A title from a WAD takes two steps
more, because of how Nintendo shipped them:

1. **Decrypt it.** A WAD carries its ticket, its TMD and its contents, and the
   contents are encrypted with the title's own key.
2. **Find the game.** The content a title *boots* is usually a small loader -
   often one shared with other titles - and the game itself is another content,
   compressed. Mega Man 9's is 1.4 MB of LZ11 that expands to 2.87 MB.

`wiinx-wad` does both:

```sh
../libdol-nx/tools/wiinx-wad "Mega Man 9.wad" unpacked/
# unpacked/main.dol     the content the title boots
# unpacked/payload.dol  the game, expanded
```

## What is here

| | Title | State |
|---|---|---|
| [megaman9-nx](megaman9-nx) | `WR9E`, WiiWare | translated: 10,321 functions, 266 natives bound. Its game is LZ11 inside a content |
| [crystaldefenders-nx](crystaldefenders-nx) | `WCIE`, WiiWare | set up: 6,596 functions, 2,500 named, 237 natives bound. Its game is a plain content |

## What to expect of a kind

**WiiWare** is an ordinary Wii game in a different wrapper: the SDK, nw4r, the
same libraries a disc game links. The set of known signatures names thousands of
its functions straight away.

**Virtual Console** is not. Each one is an emulator Nintendo shipped, and they
lean on IOS and the hardware in ways an SDK game does not. They also share
contents - the two Zelda titles carry the same three, which is the emulator -
so installing one needs the shared-content map as well.
