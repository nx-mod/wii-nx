// Punch-Out!!'s own: what libwii-nx cannot know about this game.
//
// Found in the game's own code by
// `libwii-nx/tools/wiinx-scan os-globals disc/sys/main.dol`, not by hand. The
// same scan reproduces Mario Kart Wii's known values exactly, which is the
// reason to trust these.
#include "guest_os_layout.h"

namespace {

RuntimeGuestOs::Layout GuestOsLayout() {
    RuntimeGuestOs::Layout layout;
    // The scheduler's three words, together as the SDK declares them.
    layout.scheduler_disable_count = 0x804143A8u;
    layout.reschedule = 0x804143ACu;
    layout.run_queue_bits = 0x804143B0u;
    // OSThread.c's order in memory: the thread the game boots on, the 32 run
    // queues, then the thread the scheduler idles on.
    layout.default_thread = 0x803C96D8u;
    layout.run_queue = 0x803C99F0u;
    layout.idle_thread = 0x803C9AF0u;
    // The rest is still unfound for this game and left zero; startup says so.
    return layout;
}

struct Install {
    Install() { RuntimeGuestOs::install(GuestOsLayout()); }
};
const Install g_install;

}  // namespace
