#include "hle_stubs.h"
#include "memory.h"

#include <algorithm>
#include "aurora_events.h"
#include "game_graphics_options.h"
#include "runtime_config.h"
#include "game_hooks.h"
#include "guest_os_layout.h"
#include "mkwii_dynamic_aspect_records.h"

#include <dolphin/vi.h>

// Advanced by GXCopyDisp (gx_copy.cpp), defined in gx_utils.cpp. Declared here
// rather than via gx_internal.h, which is private to the GX HLE sources.
extern "C" int g_gxFrameCount;

// Whether the canvas follows the display. Read by this file only.
extern "C" bool g_dynamicAspectRatioEnabled = false;

namespace {

bool g_widescreenConfigured = false;
uint32_t g_lastEggWidth43 = 0;
uint32_t g_lastEggWidth169 = 0;

// EGG::Screen's static canvas records (PAL). See gx_dynamic_aspect.h for the
// record layout and the anamorphic presentation model.
constexpr uint32_t kEggScreenRecord43 = 0x802A3EE8u;
constexpr uint32_t kEggScreenRecord169 = 0x802A3EF4u;
constexpr uint32_t kEggActiveScreenPtr = 0x80386F14u;

constexpr uint32_t kEggScreenProjScaleX = 0x80386F20u;
constexpr uint32_t kEggScreenProjScaleY = 0x80386F24u;

constexpr uint32_t kEggScreenAspectHandler = 0x8023E53Cu;

constexpr uint32_t kMkwUpdateAllScreens = 0x805653D0u;
constexpr uint32_t kMkwGfxDrawList = 0x809C1830u;
constexpr uint32_t kSystemManagerInstance = 0x80386000u;

// The draw list is an nw4r::ut::List: +0x00 head, +0x04 tail, +0x08 u16 count,
// +0x0A u16 link offset, with next = *(node + linkOffset + 4) (List_GetNext,
// 0x800AF180).
constexpr uint32_t kMkwGfxDrawListLinkOffset = 0x0Au;

constexpr uint32_t kMkwGfxOffscreenList = 0x809C183Cu;
constexpr uint32_t kMkwGfxOffscreenNodeScreenSlot = 0x10u;
constexpr uint32_t kEggScreenVTableOffset = 0x38u;
constexpr uint32_t kEggScreenVTable = 0x802A3F0Cu;
constexpr uint32_t kMkwScreenVTable = 0x808B4C20u;
constexpr uint32_t kEggScreenFlagsOffset = 0x34u;
constexpr uint16_t kEggScreenFlagFramebufferCanvas = 0x0008u;
constexpr uint16_t kEggScreenFlagKeepFrustumScale = 0x0040u;
constexpr uint32_t kMkwGfxNodeScreenSlots[] = {0x10u, 0x28u, 0x2Cu};

bool IsEggScreen(uint32_t address) {
    if (address == 0 || (address & 3u) != 0 || !Memory::Contains(address, 0x40u)) {
        return false;
    }
    const uint32_t vtable = Memory::Read32(address + kEggScreenVTableOffset);
    return vtable == kMkwScreenVTable || vtable == kEggScreenVTable;
}

void KeepFrustumScale(uint32_t screen) {
    if (!IsEggScreen(screen)) {
        return;
    }
    const uint16_t flags = Memory::Read16(screen + kEggScreenFlagsOffset);
    if ((flags & kEggScreenFlagKeepFrustumScale) != 0) {
        return;
    }
    Memory::Write16(screen + kEggScreenFlagsOffset,
                    static_cast<uint16_t>(flags | kEggScreenFlagKeepFrustumScale));
}

void KeepFrustumScaleOnFramebufferCanvasScreen(uint32_t screen) {
    if (!IsEggScreen(screen) ||
        (Memory::Read16(screen + kEggScreenFlagsOffset) &
         kEggScreenFlagFramebufferCanvas) == 0) {
        return;
    }
    KeepFrustumScale(screen);
}

// Walk one gfx-node list. On the on-screen list only screens that have already
// declared themselves framebuffer-canvas (bit 3) may be bypassed. On the
// offscreen list every screen is offscreen by construction, so no predicate is
// needed - and none would work anyway, because bit 3 is only set inside the
// bake itself.
void SweepGfxNodeList(uint32_t list, bool offscreenList) {
    if (!Memory::Contains(list, 0x0Cu)) {
        return;
    }
    const uint32_t linkOffset = Memory::Read16(list + kMkwGfxDrawListLinkOffset);
    // The on-screen walk also reads the menu node's parked-screen slots at
    // +0x28/+0x2C, which sit past the link words on small nodes.
    const uint32_t nodeSpan =
        std::max(linkOffset + 8u, offscreenList ? 0u : kMkwGfxNodeScreenSlots[2] + 4u);
    uint32_t node = Memory::Read32(list);
    for (int guard = 0; node != 0 && guard < 64; ++guard) {
        if (!Memory::Contains(node, nodeSpan)) {
            break;
        }
        if (offscreenList) {
            KeepFrustumScale(Memory::Read32(node + kMkwGfxOffscreenNodeScreenSlot));
        } else {
            for (const uint32_t slot : kMkwGfxNodeScreenSlots) {
                KeepFrustumScaleOnFramebufferCanvasScreen(Memory::Read32(node + slot));
            }
        }
        node = Memory::Read32(node + linkOffset + 4u);
    }
}

// Vertical expansion rides EGG::Screen's global scale, which any screen without bit 6 imports
// at projection-build time. MKW's offscreen passes only set bit 3 (fixed-size framebuffer canvas,
// record 2), so without this bypass they'd inherit the expansion and render vertically squashed.
// Re-arms every scene transition since the offscreen renderer is a per-scene singleton.
void AssertOffscreenScreenBypass() {
    SweepGfxNodeList(kMkwGfxDrawList, /*offscreenList=*/false);
    SweepGfxNodeList(kMkwGfxOffscreenList, /*offscreenList=*/true);
}

void WriteEggScreenRecord(uint32_t recordAddr, uint32_t width) {
    Memory::Write16(recordAddr, static_cast<uint16_t>(width));
    Memory::Write16(recordAddr + 0x02u,
                    static_cast<uint16_t>(MkwDynamicAspect::kEggScreenHeight));
    Memory::WriteFloat32(recordAddr + 0x04u, MkwDynamicAspect::EggHorizontalScale(width));
    Memory::WriteFloat32(recordAddr + 0x08u, 1.0f);
}

void ApplyEggScreenRecords(uint32_t surfaceWidth, uint32_t surfaceHeight) {
    using namespace MkwDynamicAspect;
    const uint32_t width43 =
        g_widescreenConfigured ? EggRecordWidth(kEggRecordWidth43, surfaceWidth, surfaceHeight)
                               : kEggRecordWidth43;
    const uint32_t width169 =
        g_widescreenConfigured ? EggRecordWidth(kEggRecordWidth169, surfaceWidth, surfaceHeight)
                               : kEggRecordWidth169;
    WriteEggScreenRecord(kEggScreenRecord43, width43);
    WriteEggScreenRecord(kEggScreenRecord169, width169);
    Memory::WriteFloat32(kEggScreenProjScaleX, 1.0f);
    Memory::WriteFloat32(kEggScreenProjScaleY,
                         g_widescreenConfigured
                             ? VerticalExpansion(surfaceWidth, surfaceHeight)
                             : 1.0f);
    AssertOffscreenScreenBypass();
    if (width43 == g_lastEggWidth43 && width169 == g_lastEggWidth169) {
        return;
    }
    g_lastEggWidth43 = width43;
    g_lastEggWidth169 = width169;
    CpuContext callbackCpu = GetPersistentCpuContext();
    if (Memory::Read32(kSystemManagerInstance) != 0 && Memory::Read32(kMkwGfxDrawList) != 0) {
        InvokeIndirectCpu(kMkwUpdateAllScreens, &callbackCpu);
        return;
    }
    if (Memory::Read32(kEggActiveScreenPtr) != 0) {
        InvokeIndirectCpu(kEggScreenAspectHandler, &callbackCpu);
    }
}

} // namespace


void AssertMkwOffscreenScreenBypass() {
    static int lastSweptFrame = -1;
    if (lastSweptFrame == g_gxFrameCount) {
        return;
    }
    lastSweptFrame = g_gxFrameCount;
    AssertOffscreenScreenBypass();
}

void UpdateMkwDynamicAspectSurface(uint32_t surfaceWidth, uint32_t surfaceHeight) {
    if (!g_widescreenConfigured || surfaceWidth == 0 || surfaceHeight == 0) {
        return;
    }
    AuroraSetViewportPolicy(AURORA_VIEWPORT_STRETCH);
    ApplyEggScreenRecords(surfaceWidth, surfaceHeight);
}

void ConfigureMkwDynamicAspect(bool widescreen, uint32_t surfaceWidth, uint32_t surfaceHeight) {
    g_widescreenConfigured = widescreen;
    g_dynamicAspectRatioEnabled = widescreen;
    g_lastEggWidth43 = 0;
    g_lastEggWidth169 = 0;
    if (widescreen) {
        VIUnlockAspectRatio();
        ApplyEggScreenRecords(surfaceWidth, surfaceHeight);
        return;
    }

    AuroraSetViewportPolicy(AURORA_VIEWPORT_FIT);
    VILockAspectRatio(4, 3);
    ApplyEggScreenRecords(surfaceWidth, surfaceHeight);
}

// ---------------------------------------------------------------------------
// This file is Mario Kart Wii's own, and the only place in the build that
// knows it: the runtime calls the hooks below, never these functions. It moves
// to that game's project (its native/ folder) once the game is built from
// libwii-nx alone. See docs/porting.md.
// ---------------------------------------------------------------------------
namespace {

// ScnRenderer::createPath in Mario Kart Wii PAL takes the post-processing path
// mask in r4. Depth of field always goes; bloom goes when the player turned it
// off (runtime setting).
constexpr uint32_t kScnRendererCreatePath = 0x8023BD38u;
constexpr uint32_t kDepthOfFieldPath = 0x20u;

void OnCalling(uint32_t target, CpuContext* ctx) {
    if (target == kScnRendererCreatePath) {
        ctx->gpr[4] &= ~(RuntimeGameGraphicsOptions::DisabledPostProcessingPaths() | kDepthOfFieldPath);
    }
}

void OnSurfaceResized(uint32_t width, uint32_t height) {
    static bool configured = false;
    if (!configured) {
        configured = true;
        ConfigureMkwDynamicAspect(RuntimeConfigFile::WidescreenEnabled(true), width, height);
        return;
    }
    UpdateMkwDynamicAspectSurface(width, height);
}

// Mario Kart Wii PAL's OS globals: where its copy of the Wii's OS put the
// scheduler's own variables. Another game's are elsewhere, which is why they
// are the game's to supply (guest_os_layout.h).
RuntimeGuestOs::Layout MkwGuestOsLayout() {
    RuntimeGuestOs::Layout layout;
    layout.run_queue = 0x803477B0u;
    layout.run_queue_bits = 0x80386920u;
    layout.reschedule = 0x8038691Cu;
    layout.scheduler_disable_count = 0x80386918u;
    layout.default_thread = 0x80347498u;
    layout.idle_thread = 0x803478B0u;
    layout.switch_thread_callback_ptr = 0x80385AE0u;
    layout.interrupt_handler_table_ptr = 0x803868F8u;
    layout.alarm_queue_r13_offset = 0x6360u;
    layout.load_context = 0x801A1F58u;
    layout.deferred_thread_entry = 0x8024373Cu;  // EGG::Thread::start
    return layout;
}

struct InstallGameHooks {
    InstallGameHooks() {
        RuntimeGuestOs::install(MkwGuestOsLayout());

        RuntimeGameHooks::Hooks hooks;
        hooks.surface_resized = &OnSurfaceResized;
        hooks.viewport_about_to_change = &AssertMkwOffscreenScreenBypass;
        hooks.calling = &OnCalling;
        RuntimeGameHooks::install(hooks);
    }
};
const InstallGameHooks g_installGameHooks;

}  // namespace
