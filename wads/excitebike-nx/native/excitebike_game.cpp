// WWRE's own: what the libraries cannot know about this title.
//
// The scheduler's globals, from `libdol-nx/tools/wiinx-scan os-globals`. The
// SDK's other variables are in ../globals.json.
#include "guest_os_layout.h"

namespace {

RuntimeGuestOs::Layout GuestOsLayout() {
    RuntimeGuestOs::Layout layout;
    layout.run_queue = 0x804B94B0u;
    layout.run_queue_bits = 0x805BE110u;
    layout.reschedule = 0x805BE10Cu;
    layout.scheduler_disable_count = 0x805BE108u;
    layout.default_thread = 0x804B9198u;
    layout.idle_thread = 0x804B95B0u;
    return layout;
}

struct Install {
    Install() { RuntimeGuestOs::install(GuestOsLayout()); }
};
const Install g_install;

}  // namespace
