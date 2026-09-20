#pragma once

// SYSCONF: the Wii's own settings, as every game reads them.
//
// One 16 KiB file at /shared2/sys/SYSCONF in the NAND, holding the settings the
// console's menu edits: language, aspect ratio, sound, screen saver, sensor bar
// position, PAL60 and the rest. wii-nx keeps one shared copy, so a change made
// once applies to every game.
//
// Layout: "SCv0", the item count, then one big-endian offset per item. Each item
// is a byte holding its type in the high 3 bits and its name length minus one in
// the low 5, then the name, then the value (arrays store their length minus one
// first). "SCed" closes the file at 0x3FFC.

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace wiinand {

inline constexpr std::size_t kSysconfSize = 0x4000;

enum class SysconfType : std::uint8_t {
    BigArray = 1,
    SmallArray = 2,
    Byte = 3,
    Short = 4,
    Long = 5,
    LongLong = 6,
    Bool = 7,
};

struct SysconfItem {
    std::string name;
    SysconfType type = SysconfType::Byte;
    std::vector<std::uint8_t> data;

    std::uint64_t Number() const;          // byte/short/long/long long/bool
    bool Boolean() const { return Number() != 0; }
};

// A whole SYSCONF, parsed or built from scratch.
class Sysconf {
  public:
    // The settings a Wii ships with, which is what a new NAND gets.
    static Sysconf Defaults();

    // Parses a file. Returns nothing when it is not a SYSCONF.
    static std::optional<Sysconf> Parse(const std::vector<std::uint8_t>& file);

    const std::vector<SysconfItem>& Items() const { return mItems; }
    const SysconfItem* Find(std::string_view name) const;

    std::optional<std::uint64_t> Number(std::string_view name) const;
    bool Boolean(std::string_view name, bool fallback = false) const;

    // Sets an existing setting, or adds one. Numbers are written in the width
    // the setting already has. False when the name exists with another type.
    bool SetNumber(std::string_view name, std::uint64_t value);
    bool SetBoolean(std::string_view name, bool value);
    void Set(SysconfItem item);

    // The 16 KiB file, ready to write to the NAND.
    std::vector<std::uint8_t> Build() const;

  private:
    std::vector<SysconfItem> mItems;
};

// Common settings, spelled once so callers do not repeat the strings.
namespace sysconf_keys {
inline constexpr std::string_view kLanguage = "IPL.LNG";        // 0 JP, 1 EN, 2 DE, 3 FR, 4 ES, 5 IT, 6 NL, 9 KO
inline constexpr std::string_view kAspectRatio = "IPL.AR";      // 0 4:3, 1 16:9
inline constexpr std::string_view kProgressive = "IPL.PGS";
inline constexpr std::string_view kPal60 = "IPL.E60";
inline constexpr std::string_view kSound = "IPL.SND";           // 0 mono, 1 stereo, 2 surround
inline constexpr std::string_view kScreenSaver = "IPL.SSV";
inline constexpr std::string_view kEulaAccepted = "IPL.EULA";
inline constexpr std::string_view kCounterBias = "IPL.CB";
inline constexpr std::string_view kSimpleAddress = "IPL.SADR";
inline constexpr std::string_view kSensorBarPosition = "BT.BAR";  // 0 below the screen, 1 above
inline constexpr std::string_view kSensorBarSensitivity = "BT.SENS";
inline constexpr std::string_view kSpeakerVolume = "BT.SPKV";
inline constexpr std::string_view kRumble = "BT.MOT";
}  // namespace sysconf_keys

}  // namespace wiinand
