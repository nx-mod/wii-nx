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
PPC_NATIVE_OVERRIDE_VOID(80183054, wiinx_native_3, (CpuContext* ctx), (ctx));

// PSMTXConcatArray
extern "C" void wiinx_native_5(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("PSMTXConcatArray", wiinx::sdk::kMtx_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(80183180, wiinx_native_5, (CpuContext* ctx), (ctx));

// memcpy
extern "C" void wiinx_native_12(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("memcpy", wiinx::sdk::kMsl_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(80004000, wiinx_native_12, (CpuContext* ctx), (ctx));

// memmove
extern "C" void wiinx_native_13(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("memmove", wiinx::sdk::kMsl_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(80154980, wiinx_native_13, (CpuContext* ctx), (ctx));

// strcmp
extern "C" void wiinx_native_16(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("strcmp", wiinx::sdk::kMsl_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(80157674, wiinx_native_16, (CpuContext* ctx), (ctx));

// strncmp
extern "C" void wiinx_native_17(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("strncmp", wiinx::sdk::kMsl_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(80157790, wiinx_native_17, (CpuContext* ctx), (ctx));

// memcmp
extern "C" void wiinx_native_23(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("memcmp", wiinx::sdk::kMsl_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(80154AA4, wiinx_native_23, (CpuContext* ctx), (ctx));

