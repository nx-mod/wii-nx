// NW4R's glyph lookup, natively, with a trace of what it is asked.
//
// nw4r::ut::ResFontBase::FindGlyphIndex(const FontCodeMap* map, u16 code) turns
// a character code into a glyph index through one CMAP block: direct (an index
// offset from the block's first code), table (one index per code) or scan (a
// sorted list of code/index pairs). 0xFFFF means the block has no glyph.
//
// The Wii Menu's web engine draws every character 31 glyphs too far into the
// font ("Need help?" as "m¡¡~ /¡“fi^"). The translated lookup is correct, so
// either the codes reach it already shifted or the right index is drawn from
// the wrong cell. The first calls are logged with their caller to tell which.
// The address is the Wii Menu's own (4.3E), so it lives with the menu.
#include "hle_stubs.h"
#include "memory.h"
#include "ppc_runtime.h"

#include <cstdint>
#include <cstdio>

#if defined(__SWITCH__)
void SwitchBootLogExternal(const char* text) noexcept;
#endif

namespace {

constexpr uint32_t kNoGlyph = 0xFFFFu;
int g_traced = 0;
constexpr int kTraceLimit = 160;

uint32_t FindGlyphIndex(uint32_t map, uint32_t code) {
    const uint32_t begin = Memory::Read16(map + 0x0);
    const uint32_t end = Memory::Read16(map + 0x2);
    const uint32_t method = Memory::Read16(map + 0x4);
    if (code < begin || code > end) {
        return kNoGlyph;
    }
    const uint32_t info = map + 0xC;
    switch (method) {
    case 0:  // direct
        return (Memory::Read16(info) + (code - begin)) & 0xFFFFu;
    case 1:  // table
        return Memory::Read16(info + 2 * (code - begin));
    case 2: {  // scan: count, then (code, index) pairs sorted by code
        uint32_t low = 0;
        uint32_t high = Memory::Read16(info);
        while (low < high) {
            const uint32_t middle = (low + high) / 2;
            const uint32_t entry = info + 2 + 4 * middle;
            const uint32_t entryCode = Memory::Read16(entry);
            if (entryCode == code) {
                return Memory::Read16(entry + 2);
            }
            if (entryCode < code) {
                low = middle + 1;
            } else {
                high = middle;
            }
        }
        return kNoGlyph;
    }
    default:
        return kNoGlyph;
    }
}

}  // namespace

// r3 = this, r4 = the CMAP block, r5 = the character code; the index in r3.
extern "C" void Nw4rFindGlyphIndex_Cpu(CpuContext* ctx) {
    const uint32_t map = ctx->gpr[4];
    const uint32_t code = ctx->gpr[5] & 0xFFFFu;
    const uint32_t index = FindGlyphIndex(map, code);
#if defined(__SWITCH__)
    if (g_traced < kTraceLimit && index != kNoGlyph) {
        ++g_traced;
        char line[160];
        std::snprintf(line, sizeof(line),
                      "[font] code 0x%04X '%c' -> glyph %u (map 0x%08X begin 0x%04X) from LR 0x%08X",
                      code, (code >= 0x20 && code < 0x7F) ? static_cast<char>(code) : '?', index, map,
                      static_cast<unsigned>(Memory::Read16(map)), ctx->lr);
        SwitchBootLogExternal(line);
    }
#endif
    ctx->gpr[3] = index;
}

PPC_NATIVE_OVERRIDE_VOID(81514A8C, Nw4rFindGlyphIndex_Cpu, (CpuContext* ctx), (ctx));
