// Written by tools/wiinx-emit-bindings. Do not edit: it is
// regenerated from this game's bindings.json every translation.
#include "hle_stubs.h"
#include "abi_bridge.h"
#include "wiinx/accel/nw4r.hpp"
#include "wiinx/accel/sdk.hpp"

// PSMTXIdentity
extern "C" void wiinx_native_3(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("PSMTXIdentity", wiinx::sdk::kMtx_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(80060F80, wiinx_native_3, (CpuContext* ctx), (ctx));

// memcpy
extern "C" void wiinx_native_12(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("memcpy", wiinx::sdk::kMsl_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(80004338, wiinx_native_12, (CpuContext* ctx), (ctx));

// memmove
extern "C" void wiinx_native_13(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("memmove", wiinx::sdk::kMsl_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(801538B0, wiinx_native_13, (CpuContext* ctx), (ctx));

// strcmp
extern "C" void wiinx_native_16(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("strcmp", wiinx::sdk::kMsl_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(80157A5C, wiinx_native_16, (CpuContext* ctx), (ctx));

// strncmp
extern "C" void wiinx_native_17(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("strncmp", wiinx::sdk::kMsl_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(80157B78, wiinx_native_17, (CpuContext* ctx), (ctx));

// strchr
extern "C" void wiinx_native_21(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("strchr", wiinx::sdk::kMsl_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(80157BB8, wiinx_native_21, (CpuContext* ctx), (ctx));

// memcmp
extern "C" void wiinx_native_23(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("memcmp", wiinx::sdk::kMsl_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(801539D4, wiinx_native_23, (CpuContext* ctx), (ctx));

