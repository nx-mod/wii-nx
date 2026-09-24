// Written by tools/wiinx-emit-bindings. Do not edit: it is
// regenerated from this game's bindings.json every translation.
#include "hle_stubs.h"
#include "abi_bridge.h"
#include "wiinx/accel/nw4r.hpp"
#include "wiinx/accel/sdk.hpp"

// THPVideoDecode
extern "C" void wiinx_native_2(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("THPVideoDecode", wiinx::sdk::kThp_2007_08.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(801B3BAC, wiinx_native_2, (CpuContext* ctx), (ctx));

// PSMTXIdentity
extern "C" void wiinx_native_3(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("PSMTXIdentity", wiinx::sdk::kMtx_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(80199D04, wiinx_native_3, (CpuContext* ctx), (ctx));

// PSMTXConcat
extern "C" void wiinx_native_4(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("PSMTXConcat", wiinx::sdk::kMtx_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(80199D64, wiinx_native_4, (CpuContext* ctx), (ctx));

// PSMTXConcatArray
extern "C" void wiinx_native_5(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("PSMTXConcatArray", wiinx::sdk::kMtx_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(80199E30, wiinx_native_5, (CpuContext* ctx), (ctx));

// PSMTXCopy
extern "C" void wiinx_native_6(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("PSMTXCopy", wiinx::sdk::kMtx_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(80199D30, wiinx_native_6, (CpuContext* ctx), (ctx));

// PSMTXTrans
extern "C" void wiinx_native_8(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("PSMTXTrans", wiinx::sdk::kMtx_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(8019A3E0, wiinx_native_8, (CpuContext* ctx), (ctx));

// PSMTXTransApply
extern "C" void wiinx_native_9(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("PSMTXTransApply", wiinx::sdk::kMtx_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(8019A414, wiinx_native_9, (CpuContext* ctx), (ctx));

// PSMTXScale
extern "C" void wiinx_native_10(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("PSMTXScale", wiinx::sdk::kMtx_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(8019A460, wiinx_native_10, (CpuContext* ctx), (ctx));

// PSMTXScaleApply
extern "C" void wiinx_native_11(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("PSMTXScaleApply", wiinx::sdk::kMtx_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(8019A488, wiinx_native_11, (CpuContext* ctx), (ctx));

// memcpy
extern "C" void wiinx_native_12(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("memcpy", wiinx::sdk::kMsl_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(80005F34, wiinx_native_12, (CpuContext* ctx), (ctx));

// memmove
extern "C" void wiinx_native_13(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("memmove", wiinx::sdk::kMsl_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(8000F1F0, wiinx_native_13, (CpuContext* ctx), (ctx));

// memset
extern "C" void wiinx_native_14(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("memset", wiinx::sdk::kMsl_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(80006038, wiinx_native_14, (CpuContext* ctx), (ctx));

// strlen
extern "C" void wiinx_native_15(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("strlen", wiinx::sdk::kMsl_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(80021254, wiinx_native_15, (CpuContext* ctx), (ctx));

// strcmp
extern "C" void wiinx_native_16(CpuContext* ctx) {
    static const wiinx::Native* const native =
        wiinx::find_native("strcmp", wiinx::sdk::kMsl_Any.id);
    native->entry(reinterpret_cast<wiinx::Cpu*>(ctx));
}
PPC_NATIVE_OVERRIDE_VOID(8001329C, wiinx_native_16, (CpuContext* ctx), (ctx));

