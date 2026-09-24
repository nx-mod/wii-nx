# Mario Kart Wii's own native code

Replacements for things only this game does. Everything the Wii's SDK and its
middleware provide lives in the engine (`wiicompiled-nx/runtime/src`) and is
bound to each game's own addresses, so nothing general belongs here.

What is here is what `example-wii-nx/scripts/audit` counts as game-specific: for
Mario Kart Wii that is two functions - the strap screen's input check and the
mod loader's entry hook - which still live in the engine and move here when
touched.

Files in this folder are compiled into this game's build alone.
