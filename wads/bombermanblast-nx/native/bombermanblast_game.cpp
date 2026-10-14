// Bomberman Blast's own: what the libraries cannot know about this title.
//
// The scheduler's globals, from `libdol-nx/tools/wiinx-scan os-globals
// game/payload.dol`. The SDK's other variables are in ../globals.json.
#include "guest_os_layout.h"

namespace {

RuntimeGuestOs::Layout GuestOsLayout() {
    RuntimeGuestOs::Layout layout;
    layout.run_queue = 0x8042BE30u;
    layout.run_queue_bits = 0x8046C030u;
    layout.reschedule = 0x8046C02Cu;
    layout.scheduler_disable_count = 0x8046C028u;
    layout.default_thread = 0x8042BB18u;
    layout.idle_thread = 0x8042BF30u;
    layout.interrupt_handler_table_ptr = 0x8046C008u;
    layout.alarm_queue_r13_offset = 0x00006328u;
    layout.load_context = 0x802349A8u;
    return layout;
}

struct Install {
    Install() { RuntimeGuestOs::install(GuestOsLayout()); }
};
const Install g_install;

}  // namespace
