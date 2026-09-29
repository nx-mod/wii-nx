// WRXE's own: what the libraries cannot know about this title.
//
// The scheduler's globals, from `libdol-nx/tools/wiinx-scan os-globals`. The
// SDK's other variables are in ../globals.json.
#include "guest_os_layout.h"

namespace {

RuntimeGuestOs::Layout GuestOsLayout() {
    RuntimeGuestOs::Layout layout;
    layout.run_queue = 0x805C8930u;
    layout.run_queue_bits = 0x8067CD00u;
    layout.reschedule = 0x8067CCFCu;
    layout.scheduler_disable_count = 0x8067CCF8u;
    layout.default_thread = 0x805C8618u;
    layout.idle_thread = 0x805C8A30u;
    return layout;
}

struct Install {
    Install() { RuntimeGuestOs::install(GuestOsLayout()); }
};
const Install g_install;

}  // namespace
