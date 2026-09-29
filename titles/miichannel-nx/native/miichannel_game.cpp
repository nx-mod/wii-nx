// Title 0001000248414341's own: what the libraries cannot know about it.
//
// The scheduler's globals, from `libdol-nx/tools/wiinx-scan os-globals`. The
// SDK's other variables are in ../globals.json.
#include "guest_os_layout.h"

namespace {

RuntimeGuestOs::Layout GuestOsLayout() {
    RuntimeGuestOs::Layout layout;
    layout.run_queue = 0x8035EDB0u;
    layout.run_queue_bits = 0x803BDB38u;
    layout.reschedule = 0x803BDB34u;
    layout.scheduler_disable_count = 0x803BDB30u;
    layout.default_thread = 0x8035EA98u;
    layout.idle_thread = 0x8035EEB0u;
    return layout;
}

struct Install {
    Install() { RuntimeGuestOs::install(GuestOsLayout()); }
};
const Install g_install;

}  // namespace
