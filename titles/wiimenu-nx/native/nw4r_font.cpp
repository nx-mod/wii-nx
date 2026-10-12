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
#include "abi_bridge.h"
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

// The font the web engine reads by name: WWW_FONT_FILE_DATA_TABLE__ (16-byte
// entries: name, -, data, end), filled by the menu's SetFontData. Polled once a
// second from the retrace hook; logged when its data appears, or if it never
// has by 90 s.
void PollWebFontTable(CpuContext*) {
#if defined(__SWITCH__)
    static int frames = 0;
    static bool done = false;
    if (done || (++frames % 60) != 0) {
        return;
    }
    try {
        const uint32_t entry = 0x81643C4Cu;
        const uint32_t data = Memory::Read32(entry + 8), end = Memory::Read32(entry + 12);
        if (data == 0 && frames < 90 * 60) {
            return;
        }
        done = true;
        char text[24] = {};
        const uint32_t name = Memory::Read32(entry);
        for (int i = 0; i < 23 && name >= 0x80000000u; ++i) {
            text[i] = static_cast<char>(Memory::Read8(name + i));
            if (!text[i]) break;
        }
        std::string bytes;
        for (uint32_t i = 0; data >= 0x80000000u && i < 32; ++i) {
            char hex[4];
            std::snprintf(hex, sizeof(hex), "%02X", Memory::Read8(data + i));
            bytes += hex;
        }
        char line[220];
        std::snprintf(line, sizeof(line), "[font] web font '%s' at %d s: data 0x%08X size 0x%X: %s", text,
                      frames / 60, data, end - data, bytes.c_str());
        SwitchBootLogExternal(line);
    } catch (const Memory::AccessViolation&) {
    }
#endif
}

const bool g_webFontPoll = [] {
    VI_HLE_AddRetraceHook(PollWebFontTable);
    return true;
}();

// r3 = this, r4 = the CMAP block, r5 = the character code; the index in r3.
// The font object as the menu's archive font lays it out: +20 its FINF, whose
// +8 is the TGLP (glyph sheets); +24 the per-sheet index adjustment table that
// GetGlyphFromIndex subtracts (0xFFFF = sheet not loaded).
void DumpFontOnce(uint32_t font) {
#if defined(__SWITCH__)
    static bool done = false;
    if (done || font < 0x80000000u) {
        return;
    }
    done = true;
    char line[200];
    const uint32_t finf = Memory::Read32(font + 20);
    const uint32_t tglp = Memory::Read32(finf + 8);
    const uint32_t table = Memory::Read32(font + 24);
    const uint32_t cols = Memory::Read16(tglp + 12), rows = Memory::Read16(tglp + 14);
    const uint32_t sheets = Memory::Read16(tglp + 8);
    std::snprintf(line, sizeof(line),
                  "[font] object 0x%08X finf 0x%08X tglp 0x%08X: cell %ux%u sheetSize 0x%X sheets %u fmt 0x%X "
                  "cols %u rows %u image 0x%08X alter %u table 0x%08X",
                  font, finf, tglp, Memory::Read8(tglp), Memory::Read8(tglp + 1), Memory::Read32(tglp + 4), sheets,
                  Memory::Read16(tglp + 10), cols, rows, Memory::Read32(tglp + 20), Memory::Read16(finf + 2), table);
    SwitchBootLogExternal(line);
#else
    (void)font;
#endif
}

extern "C" void Nw4rFindGlyphIndex_Cpu(CpuContext* ctx) {
    const uint32_t map = ctx->gpr[4];
    const uint32_t code = ctx->gpr[5] & 0xFFFFu;
    const uint32_t index = FindGlyphIndex(map, code);
    try {
        DumpFontOnce(ctx->gpr[3]);
    } catch (const Memory::AccessViolation&) {
    }
#if defined(__SWITCH__)
    // The layout path (GetCharWidths, whose call returns to 0x8151506C) asks
    // for every character once to build a width table; the drawing path
    // (GetGlyph, returning to 0x81516B74) is the one that shows text. Only the
    // drawing path is traced, so its lookups are not crowded out.
    uint32_t caller = 0;
    try {
        const uint32_t frame = Memory::Read32(ctx->gpr[1]);
        caller = Memory::Read32(frame + 4);
    } catch (const Memory::AccessViolation&) {
    }
    const bool drawing = caller != 0x8151506Cu;
    if (drawing && g_traced < kTraceLimit && index != kNoGlyph) {
        ++g_traced;
        char line[160];
        std::snprintf(line, sizeof(line),
                      "[font] code 0x%04X '%c' -> glyph %u (map 0x%08X begin 0x%04X) from LR 0x%08X",
                      code, (code >= 0x20 && code < 0x7F) ? static_cast<char>(code) : '?', index, map,
                      static_cast<unsigned>(Memory::Read16(map)), ctx->lr);
        SwitchBootLogExternal(line);
        // Who asked: the guest call chain, by the back chain of stack frames
        // (each frame's word 0 is the caller's r1, the caller's frame keeps
        // the return address at +4). For the first characters only.
        if (g_traced <= 6) {
            std::string chain = "[font]   called from";
            uint32_t sp = ctx->gpr[1];
            for (int depth = 0; depth < 8; ++depth) {
                const uint32_t prev = Memory::Read32(sp);
                if (prev <= sp || prev - sp > 0x10000u) {
                    break;
                }
                char hop[16];
                std::snprintf(hop, sizeof(hop), " 0x%08X", Memory::Read32(prev + 4));
                chain += hop;
                sp = prev;
            }
            SwitchBootLogExternal(chain.c_str());
        }
    }
#endif
    ctx->gpr[3] = index;
}

PPC_NATIVE_OVERRIDE_VOID(81514A8C, Nw4rFindGlyphIndex_Cpu, (CpuContext* ctx), (ctx));

// FreeType in the web engine (wwwlib, 4.3E addresses). Opera draws page text
// with FreeType from the system TrueType font (content 0x0A), and in this
// runtime every character came out as glyph index = character code - what
// FreeType callers fall back to when the face has no Unicode charmap. These
// two do what FreeType 2.2 does and log what they see.
//
// find_unicode_charmap(face): face+36 num_charmaps, face+40 charmaps (array of
// FT_CharMap: +4 encoding, +8 platform_id, +10 encoding_id), face+92 charmap.
extern "C" void FtFindUnicodeCharmap_Cpu(CpuContext* ctx) {
    constexpr uint32_t kUnicode = 0x756E6963u;  // 'unic'
    const uint32_t face = ctx->gpr[3];
    uint32_t result = 0x26;  // FT_Err_Invalid_CharMap_Handle
    try {
        const uint32_t charmaps = Memory::Read32(face + 40);
        const uint32_t count = Memory::Read32(face + 36);
        uint32_t chosen = 0;
#if defined(__SWITCH__)
        static int logged = 0;
        const bool log = logged++ < 4;
#endif
        for (uint32_t i = 0; charmaps && i < count && i < 16; ++i) {
            const uint32_t map = Memory::Read32(charmaps + 4 * i);
            const uint32_t encoding = Memory::Read32(map + 4);
#if defined(__SWITCH__)
            if (log) {
                char line[120];
                std::snprintf(line, sizeof(line), "[ft] face 0x%08X charmap %u: encoding 0x%08X platform %u id %u",
                              face, i, encoding, Memory::Read16(map + 8), Memory::Read16(map + 10));
                SwitchBootLogExternal(line);
            }
#endif
            // UCS-4 (3,10) first, otherwise the last Unicode one, as FreeType does.
            if (encoding == kUnicode && (chosen == 0 || (Memory::Read16(map + 8) == 3 && Memory::Read16(map + 10) == 10))) {
                chosen = map;
            }
        }
        if (chosen) {
            Memory::Write32(face + 92, chosen);
            result = 0;
        }
#if defined(__SWITCH__)
        if (log) {
            char line[96];
            std::snprintf(line, sizeof(line), "[ft] face 0x%08X: %u charmap(s), unicode %s", face, count,
                          chosen ? "selected" : "NOT FOUND");
            SwitchBootLogExternal(line);
        }
#endif
    } catch (const Memory::AccessViolation&) {
    }
    ctx->gpr[3] = result;
}

PPC_NATIVE_OVERRIDE_VOID(8008ED2C, FtFindUnicodeCharmap_Cpu, (CpuContext* ctx), (ctx));

// FT_Get_Char_Index(face, charcode): face->charmap's class char_index (cmap+12
// is its class, whose +12 is char_index), 0 without a charmap.
extern "C" void FtGetCharIndex_Cpu(CpuContext* ctx) {
    const uint32_t face = ctx->gpr[3];
    const uint32_t code = ctx->gpr[4];
    uint32_t glyph = 0;
    uint32_t charmap = 0;
    try {
        charmap = face ? Memory::Read32(face + 92) : 0;
        if (charmap) {
            const uint32_t clazz = Memory::Read32(charmap + 12);
            const uint32_t charIndex = Memory::Read32(clazz + 12);
            const uint32_t savedLr = ctx->lr;
            ctx->gpr[3] = charmap;
            ctx->gpr[4] = code;
            InvokeIndirectCpu(charIndex, ctx);
            ctx->lr = savedLr;
            glyph = ctx->gpr[3];
        }
    } catch (const Memory::AccessViolation&) {
    }
#if defined(__SWITCH__)
    static int logged = 0;
    if (logged < 24 && code >= 0x20) {
        ++logged;
        char line[96];
        std::snprintf(line, sizeof(line), "[ft] char 0x%04X '%c' -> glyph %u (charmap 0x%08X)", code,
                      code < 0x7F ? static_cast<char>(code) : '?', glyph, charmap);
        SwitchBootLogExternal(line);
    }
#endif
    ctx->gpr[3] = glyph;
}

PPC_NATIVE_OVERRIDE_VOID(800909D4, FtGetCharIndex_Cpu, (CpuContext* ctx), (ctx));
