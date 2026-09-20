// Mario Kart Wii's mod loader hooks.
//
// Retro Rewind and other mods load an extra code module (StaticR.rel) that this
// game's own startup pulls in. Two of its functions are wrapped so the module's
// initializers run at the right moments - which is particular to this game, not
// something the Wii's SDK does.
#include "abi_bridge.h"
#include "recomp_mod_loader.h"

extern "C" void func_801A961C(CpuContext* ctx);
extern "C" void func_8055531C(CpuContext* ctx);

// Late init: after the guest's own alarm setup.
extern "C" void OSInitAlarm_RecompModLateInit_801a961c(CpuContext* ctx) {
    func_801A961C(ctx);
}

REGISTER_NATIVE_FUNCTION_AS(0x801A961C, OSInitAlarm_RecompModLateInit_801a961c, "OSInitAlarm_RecompModLateInit_801a961c");

// The module's own prologue: memory initializers before it, the rest after.
extern "C" void StaticRProlog_RecompModInit_8055531c(CpuContext* ctx) {
    RecompMod::RunMemoryInitializers();
    func_8055531C(ctx);
    RecompMod::RunPostRelInitializers();
}

REGISTER_NATIVE_FUNCTION_AS(0x8055531C, StaticRProlog_RecompModInit_8055531c, "StaticRProlog_RecompModInit_8055531c");
