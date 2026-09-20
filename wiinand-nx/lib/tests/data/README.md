# Reference files

`SYSCONF` and `RFL_DB.dat` here were written by wiicompiled-nx's own first-run code
(`runtime/include/nand_first_run.h`), so the test checks this library against
what the runtime actually produces rather than against itself.

They contain no Nintendo data: they are the settings block and the empty Mii
database a blank Wii would have, generated from our own source.

Format details come from the public documentation (wiibrew.org) and from
Dolphin's source read as reference; the code here is our own.
