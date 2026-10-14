# Status

## 2026-10-14: Mega Man 9 plays

The second title on native graphics, and the first WAD: start-up, menus and
stages at 57-59 fps. What it needed is in
[wads/megaman9-nx/README.md](wads/megaman9-nx/README.md); every fix is in the
libraries, so the other titles get them too:
- **Translator:** native register contracts were keyed by Mario Kart Wii's
  addresses. Each title's own functions at those addresses were translated as
  the native (1 to 15 functions per title). Every title needs a retranslate.
- **Graphics:** presented at the flip (no flicker when a game copies twice a
  frame), line width and point size applied, the next frame's EFB clear follows
  the copy's flag.
- **OS:** interrupts restored per thread on a fiber switch; `/dev/es` and
  `NANDInit` set up for real.

Bomberman Blast (USA) is translated with the Wiimmfi patch, not built yet.

## 2026-10-13: Mario Kart Wii on native graphics

libdol-nx's deko3d renderer is the default; Aurora and Dawn are not linked.

| | |
|---|---|
| Runs | boots, menus, the course flyover, races, saves, music and sound |
| Speed | menus faster but not 60; races about 12 fps (an ~80 ms frame) |
| Broken | Miis (no heads; the face texture loses its eyes and mouth), the race minimap |
| Off | async disc reads (a hang between menus, not found yet) |

Fixed since 2026-10-12:
- **Models, flyover and race intro:** natives that end a batch of GX state now
  run the SDK's dirty-state flush, and an EFB copy clears only its source
  rectangle, through the update masks, as the hardware does.
- **Black bars:** the display copy's 608x456 source is stretched to the screen.
- **Faster:** a render thread on its own core; draws merged into indexed
  lists; register writes that change nothing skipped; compiled shaders kept on
  the card (`cache/shaders.pack`); boot's disc scan from 4.8 s to 0.13 s; the
  log buffered.

In build 52, under test:
- guest memory accesses inlined (~15% of a race frame went to out-of-line
  checked calls), audio output on core 1, threads placed on fixed cores,
  diagnostics off, a frame-rate overlay (top left).

Where a race frame goes, and what is next: libdol-nx
[docs/performance.md](https://github.com/nx-mod/libdol-nx/blob/main/docs/performance.md).
The renderer and what is open in it:
[docs/deko3d.md](https://github.com/nx-mod/libdol-nx/blob/main/docs/deko3d.md).

## Other titles

- **Wii Party** is set up as game 2 (`games/wiiparty-nx`).
- **Prior art:** [wiiparty-recomp](https://github.com/arelkair/wiiparty-recomp)
  (native GX, recompiled DSP) and the
  [Strikers fork](https://github.com/BlackAndBlue95/Strikers-WiiCompiled)
  (translator fixes) are GPL-3.0, so their code can come here.
