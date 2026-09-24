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
PPC_NATIVE_OVERRIDE_VOID(801C04D0, wiinx_native_3, (CpuContext* ctx), (ctx));

// PSMTXConcat
extern "C" void wiinx_native_4(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("PSMTXConcat", wiinx::sdk::kMtx_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(801C0540, wiinx_native_4, (CpuContext* ctx), (ctx));

// PSMTXConcatArray
extern "C" void wiinx_native_5(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("PSMTXConcatArray", wiinx::sdk::kMtx_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(801C0610, wiinx_native_5, (CpuContext* ctx), (ctx));

// PSMTXCopy
extern "C" void wiinx_native_6(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("PSMTXCopy", wiinx::sdk::kMtx_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(801C0500, wiinx_native_6, (CpuContext* ctx), (ctx));

// PSMTXTrans
extern "C" void wiinx_native_8(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("PSMTXTrans", wiinx::sdk::kMtx_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(801C0BD0, wiinx_native_8, (CpuContext* ctx), (ctx));

// PSMTXTransApply
extern "C" void wiinx_native_9(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("PSMTXTransApply", wiinx::sdk::kMtx_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(801C0C10, wiinx_native_9, (CpuContext* ctx), (ctx));

// PSMTXScale
extern "C" void wiinx_native_10(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("PSMTXScale", wiinx::sdk::kMtx_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(801C0C60, wiinx_native_10, (CpuContext* ctx), (ctx));

// PSMTXScaleApply
extern "C" void wiinx_native_11(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("PSMTXScaleApply", wiinx::sdk::kMtx_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(801C0C90, wiinx_native_11, (CpuContext* ctx), (ctx));

// memcpy
extern "C" void wiinx_native_12(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("memcpy", wiinx::sdk::kMsl_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(80004364, wiinx_native_12, (CpuContext* ctx), (ctx));

// memmove
extern "C" void wiinx_native_13(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("memmove", wiinx::sdk::kMsl_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(802DEF74, wiinx_native_13, (CpuContext* ctx), (ctx));

// memset
extern "C" void wiinx_native_14(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("memset", wiinx::sdk::kMsl_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(800046B4, wiinx_native_14, (CpuContext* ctx), (ctx));

// strlen
extern "C" void wiinx_native_15(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("strlen", wiinx::sdk::kMsl_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(8018D3C0, wiinx_native_15, (CpuContext* ctx), (ctx));

// strcmp
extern "C" void wiinx_native_16(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("strcmp", wiinx::sdk::kMsl_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(802E1AB4, wiinx_native_16, (CpuContext* ctx), (ctx));

