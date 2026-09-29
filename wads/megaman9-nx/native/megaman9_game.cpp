// Mega Man 9's own: what the libraries cannot know about this game.
//
// The scheduler's globals, from `libdol-nx/tools/wiinx-scan os-globals
// game/payload.dol`. The SDK's VI and GX variables are in ../globals.json.
#include "guest_os_layout.h"

namespace {

RuntimeGuestOs::Layout GuestOsLayout() {
    RuntimeGuestOs::Layout layout;
    layout.scheduler_disable_count = 0x804EEA80u;
    layout.reschedule = 0x804EEA84u;
    layout.run_queue_bits = 0x804EEA88u;
    layout.default_thread = 0x8043D718u;
    layout.run_queue = 0x8043DA30u;
    layout.idle_thread = 0x8043DB30u;
    return layout;
}

struct Install {
    Install() { RuntimeGuestOs::install(GuestOsLayout()); }
};
const Install g_install;

}  // namespace
