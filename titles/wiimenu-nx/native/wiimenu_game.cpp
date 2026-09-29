// Title 0000000100000002's own: what the libraries cannot know about it.
//
// The scheduler's globals, from `libdol-nx/tools/wiinx-scan os-globals`. The
// SDK's other variables are in ../globals.json.
#include "guest_os_layout.h"

namespace {

RuntimeGuestOs::Layout GuestOsLayout() {
    RuntimeGuestOs::Layout layout;
    layout.run_queue = 0x811155F0u;
    layout.run_queue_bits = 0x8169AF28u;
    layout.reschedule = 0x8169AF24u;
    layout.scheduler_disable_count = 0x8169AF20u;
    layout.default_thread = 0x811152D8u;
    layout.idle_thread = 0x811156F0u;
    return layout;
}

struct Install {
    Install() { RuntimeGuestOs::install(GuestOsLayout()); }
};
const Install g_install;

}  // namespace
