// Retro Rewind: Mario Kart Wii with the Retro Rewind pack's own files in place
// of the disc's. One product definition, linked into the RetroRewind
// executable alone; the runtime itself is the same build as the base game's.
#include "runtime_product.h"

namespace RuntimeProduct {

namespace {
// Retro Rewind's code checks this word to tell a recompiled runtime from
// Dolphin, and takes the paths that suit one.
constexpr BootMarker kMarkers[]{
    {0x800017D8u, 0x00000001u, "Retro Rewind recomp runtime marker"},
};
}  // namespace

const Descriptor& Active() noexcept {
    static constexpr Descriptor descriptor{
        "Retro Rewind",
        "retro_rewind",
        /*overlaysDisc=*/true,
        "retro_rewind_root",
        kMarkers,
    };
    return descriptor;
}

} // namespace RuntimeProduct
