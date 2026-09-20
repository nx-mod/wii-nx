#include "wiinand/sysconf.h"

#include <algorithm>
#include <cstring>

namespace wiinand {
namespace {

constexpr std::size_t kOffsetTable = 0x0006;
constexpr std::size_t kFooterOffset = 0x3FFC;
constexpr std::size_t kPayloadLimit = 0x3FAE;   // where the trailing table begins

std::size_t FixedWidth(SysconfType type) {
    switch (type) {
        case SysconfType::Byte:
        case SysconfType::Bool:
            return 1;
        case SysconfType::Short:
            return 2;
        case SysconfType::Long:
            return 4;
        case SysconfType::LongLong:
            return 8;
        default:
            return 0;                            // arrays carry their own length
    }
}

std::uint16_t Read16(const std::vector<std::uint8_t>& file, std::size_t offset) {
    return static_cast<std::uint16_t>((file[offset] << 8) | file[offset + 1]);
}

void Write16(std::vector<std::uint8_t>& file, std::size_t offset, std::uint16_t value) {
    file[offset] = static_cast<std::uint8_t>(value >> 8);
    file[offset + 1] = static_cast<std::uint8_t>(value & 0xFF);
}

std::vector<std::uint8_t> NumberBytes(std::uint64_t value, std::size_t width) {
    std::vector<std::uint8_t> bytes(width);
    for (std::size_t index = 0; index < width; index++) {
        bytes[width - 1 - index] = static_cast<std::uint8_t>(value >> (index * 8));
    }
    return bytes;
}

}  // namespace

std::uint64_t SysconfItem::Number() const {
    std::uint64_t value = 0;
    for (const std::uint8_t byte : data) {
        value = (value << 8) | byte;
    }
    return value;
}

// static
Sysconf Sysconf::Defaults() {
    const auto byte = [](std::string name, std::uint8_t value) {
        return SysconfItem{std::move(name), SysconfType::Byte, {value}};
    };
    const auto boolean = [](std::string name, bool value) {
        return SysconfItem{std::move(name), SysconfType::Bool, {static_cast<std::uint8_t>(value)}};
    };

    Sysconf sysconf;
    sysconf.mItems = {
        SysconfItem{"BT.SENS", SysconfType::Long, NumberBytes(3, 4)},
        byte("BT.BAR", 1),                       // sensor bar above the screen
        byte("BT.SPKV", 0x58),
        boolean("BT.MOT", true),                 // rumble
        byte("IPL.AR", 1),                       // 16:9
        byte("IPL.LNG", 1),                      // English
        boolean("IPL.E60", true),                // PAL60; callers set it per region
        boolean("IPL.PGS", false),
        byte("IPL.SND", 1),                      // stereo
        byte("IPL.SSV", 0),
        boolean("IPL.EULA", true),
        boolean("IPL.UPT", true),
        SysconfItem{"IPL.CB", SysconfType::Long, NumberBytes(0, 4)},
        // Present because games read it; an empty block is a valid answer.
        SysconfItem{"IPL.SADR", SysconfType::BigArray, std::vector<std::uint8_t>(0x1007, 0)},
    };
    return sysconf;
}

// static
std::optional<Sysconf> Sysconf::Parse(const std::vector<std::uint8_t>& file) {
    if (file.size() < kFooterOffset + 4 || std::memcmp(file.data(), "SCv0", 4) != 0) {
        return std::nullopt;
    }
    Sysconf sysconf;
    const std::uint16_t count = Read16(file, 0x0004);
    for (std::uint16_t index = 0; index < count; index++) {
        const std::size_t entry = kOffsetTable + index * 2;
        if (entry + 1 >= file.size()) {
            break;
        }
        std::size_t offset = Read16(file, entry);
        if (offset == 0 || offset >= file.size()) {
            continue;
        }
        const std::uint8_t header = file[offset++];
        const auto type = static_cast<SysconfType>(header >> 5);
        const std::size_t nameLength = (header & 0x1F) + 1;
        if (offset + nameLength > file.size()) {
            break;
        }
        SysconfItem item;
        item.type = type;
        item.name.assign(reinterpret_cast<const char*>(file.data() + offset), nameLength);
        offset += nameLength;

        std::size_t length = FixedWidth(type);
        if (type == SysconfType::BigArray) {
            if (offset + 2 > file.size()) {
                break;
            }
            length = Read16(file, offset) + 1u;
            offset += 2;
        } else if (type == SysconfType::SmallArray) {
            length = file[offset] + 1u;
            offset += 1;
        }
        if (offset + length > file.size()) {
            break;
        }
        item.data.assign(file.begin() + static_cast<std::ptrdiff_t>(offset),
                         file.begin() + static_cast<std::ptrdiff_t>(offset + length));
        sysconf.mItems.push_back(std::move(item));
    }
    return sysconf;
}

const SysconfItem* Sysconf::Find(std::string_view name) const {
    const auto found = std::find_if(mItems.begin(), mItems.end(),
                                    [name](const SysconfItem& item) { return item.name == name; });
    return found == mItems.end() ? nullptr : &*found;
}

std::optional<std::uint64_t> Sysconf::Number(std::string_view name) const {
    const SysconfItem* item = Find(name);
    if (item == nullptr || FixedWidth(item->type) == 0) {
        return std::nullopt;
    }
    return item->Number();
}

bool Sysconf::Boolean(std::string_view name, bool fallback) const {
    const std::optional<std::uint64_t> value = Number(name);
    return value ? *value != 0 : fallback;
}

bool Sysconf::SetNumber(std::string_view name, std::uint64_t value) {
    for (SysconfItem& item : mItems) {
        if (item.name != name) {
            continue;
        }
        const std::size_t width = FixedWidth(item.type);
        if (width == 0) {
            return false;                        // an array, not a number
        }
        item.data = NumberBytes(value, width);
        return true;
    }
    mItems.push_back(SysconfItem{std::string(name), SysconfType::Long, NumberBytes(value, 4)});
    return true;
}

bool Sysconf::SetBoolean(std::string_view name, bool value) {
    for (SysconfItem& item : mItems) {
        if (item.name != name) {
            continue;
        }
        if (item.type != SysconfType::Bool && FixedWidth(item.type) == 0) {
            return false;
        }
        item.data.assign(1, static_cast<std::uint8_t>(value));
        return true;
    }
    mItems.push_back(SysconfItem{std::string(name), SysconfType::Bool, {static_cast<std::uint8_t>(value)}});
    return true;
}

void Sysconf::Set(SysconfItem item) {
    for (SysconfItem& existing : mItems) {
        if (existing.name == item.name) {
            existing = std::move(item);
            return;
        }
    }
    mItems.push_back(std::move(item));
}

std::vector<std::uint8_t> Sysconf::Build() const {
    std::vector<std::uint8_t> file(kSysconfSize, 0);
    std::memcpy(file.data(), "SCv0", 4);
    Write16(file, 0x0004, static_cast<std::uint16_t>(mItems.size()));

    // The offset table is terminated by a zero entry, so it needs one more slot.
    std::size_t cursor = kOffsetTable + (mItems.size() + 1) * sizeof(std::uint16_t);
    for (std::size_t index = 0; index < mItems.size(); index++) {
        const SysconfItem& item = mItems[index];
        const std::size_t extra = item.type == SysconfType::BigArray     ? 2
                                  : item.type == SysconfType::SmallArray ? 1
                                                                         : 0;
        if (cursor + 1 + item.name.size() + extra + item.data.size() > kPayloadLimit) {
            break;                               // never run into the trailing table
        }
        Write16(file, kOffsetTable + index * sizeof(std::uint16_t), static_cast<std::uint16_t>(cursor));

        file[cursor++] = static_cast<std::uint8_t>((static_cast<std::uint8_t>(item.type) << 5) |
                                                   ((item.name.size() - 1) & 0x1F));
        std::memcpy(file.data() + cursor, item.name.data(), item.name.size());
        cursor += item.name.size();

        // Array lengths are stored as "length minus one", like the name length.
        if (item.type == SysconfType::BigArray) {
            Write16(file, cursor, static_cast<std::uint16_t>(item.data.size() - 1));
            cursor += 2;
        } else if (item.type == SysconfType::SmallArray) {
            file[cursor++] = static_cast<std::uint8_t>(item.data.size() - 1);
        }
        std::memcpy(file.data() + cursor, item.data.data(), item.data.size());
        cursor += item.data.size();
    }

    std::memcpy(file.data() + kFooterOffset, "SCed", 4);
    return file;
}

}  // namespace wiinand
