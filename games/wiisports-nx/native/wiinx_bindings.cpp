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
PPC_NATIVE_OVERRIDE_VOID(800E75F8, wiinx_native_3, (CpuContext* ctx), (ctx));

// PSMTXConcat
extern "C" void wiinx_native_4(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("PSMTXConcat", wiinx::sdk::kMtx_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(800E7658, wiinx_native_4, (CpuContext* ctx), (ctx));

// PSMTXConcatArray
extern "C" void wiinx_native_5(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("PSMTXConcatArray", wiinx::sdk::kMtx_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(800E7724, wiinx_native_5, (CpuContext* ctx), (ctx));

// PSMTXCopy
extern "C" void wiinx_native_6(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("PSMTXCopy", wiinx::sdk::kMtx_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(800E7624, wiinx_native_6, (CpuContext* ctx), (ctx));

// PSMTXTranspose
extern "C" void wiinx_native_7(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("PSMTXTranspose", wiinx::sdk::kMtx_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(800E78BC, wiinx_native_7, (CpuContext* ctx), (ctx));

// PSMTXTrans
extern "C" void wiinx_native_8(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("PSMTXTrans", wiinx::sdk::kMtx_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(800E7D24, wiinx_native_8, (CpuContext* ctx), (ctx));

// PSMTXTransApply
extern "C" void wiinx_native_9(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("PSMTXTransApply", wiinx::sdk::kMtx_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(800E7D58, wiinx_native_9, (CpuContext* ctx), (ctx));

// PSMTXScale
extern "C" void wiinx_native_10(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("PSMTXScale", wiinx::sdk::kMtx_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(800E7DA4, wiinx_native_10, (CpuContext* ctx), (ctx));

// PSMTXScaleApply
extern "C" void wiinx_native_11(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("PSMTXScaleApply", wiinx::sdk::kMtx_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(800E7DCC, wiinx_native_11, (CpuContext* ctx), (ctx));

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
PPC_NATIVE_OVERRIDE_VOID(800B4DF8, wiinx_native_13, (CpuContext* ctx), (ctx));

// memset
extern "C" void wiinx_native_14(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("memset", wiinx::sdk::kMsl_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(80004104, wiinx_native_14, (CpuContext* ctx), (ctx));

// strlen
extern "C" void wiinx_native_15(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("strlen", wiinx::sdk::kMsl_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(800B1750, wiinx_native_15, (CpuContext* ctx), (ctx));

// strcmp
extern "C" void wiinx_native_16(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("strcmp", wiinx::sdk::kMsl_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(800B8DB0, wiinx_native_16, (CpuContext* ctx), (ctx));

