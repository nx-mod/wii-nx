// New Super Mario Bros. Wii's own: what libwii-nx cannot know about this game.
//
// Everything here was found in the game's own code by
// `libwii-nx/tools/wiinx-scan os-globals disc/sys/main.dol`, not by hand. The
// same scan on Mario Kart Wii reproduces that game's known values exactly,
// which is the reason to trust these.
#include "guest_os_layout.h"

namespace {

RuntimeGuestOs::Layout GuestOsLayout() {
    RuntimeGuestOs::Layout layout;
    // The scheduler's three words, together as the SDK declares them.
    layout.scheduler_disable_count = 0x8042A700u;
    layout.reschedule = 0x8042A704u;
    layout.run_queue_bits = 0x8042A708u;
    // OSThread.c's order in memory: the thread the game boots on, the 32 run
    // queues, then the thread the scheduler idles on.
    layout.default_thread = 0x8038E938u;
    layout.run_queue = 0x8038EC50u;
    layout.idle_thread = 0x8038ED50u;
    // Still unfound for this game, and left zero: the switch-thread callback
    // and interrupt table slots, the alarm queue's r13 offset, OSLoadContext,
    // and whether this game has a deferred thread entry at all. Nothing needs
    // them until its OS natives bind.
    return layout;
}

struct Install {
    Install() { RuntimeGuestOs::install(GuestOsLayout()); }
};
const Install g_install;

}  // namespace
