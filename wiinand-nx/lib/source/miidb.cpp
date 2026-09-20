#include "wiinand/miidb.h"

#include <algorithm>
#include <cstring>

namespace wiinand {
namespace {

constexpr std::size_t kEntriesOffset = 0x04;

// Offsets inside a Mii's 74 bytes. The console's own structure: two bytes of
// flags, the name, size, identifiers, the face, and finally who made it.
constexpr std::size_t kFlagsOffset = 0x00;
constexpr std::size_t kNameOffset = 0x02;
constexpr std::size_t kNameLength = 10;             // characters, UTF-16
constexpr std::size_t kIdOffset = 0x18;
constexpr std::size_t kCreatorOffset = 0x36;

constexpr std::uint16_t kInvalidFlag = 0x8000;      // set while a slot is empty
constexpr std::uint16_t kFemaleFlag = 0x4000;

std::uint16_t Read16(const std::uint8_t* data) {
    return static_cast<std::uint16_t>((data[0] << 8) | data[1]);
}

// The file stores names as big-endian UTF-16, padded with zeros. Only the Basic
// Multilingual Plane appears in practice, so this keeps the conversion simple
// and leaves anything it cannot represent as '?' rather than guessing.
std::string Utf16ToUtf8(const std::uint8_t* data, std::size_t characters) {
    std::string text;
    for (std::size_t index = 0; index < characters; index++) {
        const std::uint16_t unit = Read16(data + index * 2);
        if (unit == 0) {
            break;
        }
        if (unit < 0x80) {
            text.push_back(static_cast<char>(unit));
        } else if (unit < 0x800) {
            text.push_back(static_cast<char>(0xC0 | (unit >> 6)));
            text.push_back(static_cast<char>(0x80 | (unit & 0x3F)));
        } else if (unit >= 0xD800 && unit <= 0xDFFF) {
            text.push_back('?');                    // half of a surrogate pair
        } else {
            text.push_back(static_cast<char>(0xE0 | (unit >> 12)));
            text.push_back(static_cast<char>(0x80 | ((unit >> 6) & 0x3F)));
            text.push_back(static_cast<char>(0x80 | (unit & 0x3F)));
        }
    }
    return text;
}

}  // namespace

std::uint16_t Crc16Ccitt(const std::uint8_t* data, std::size_t length) {
    std::uint16_t crc = 0;
    for (std::size_t index = 0; index < length; index++) {
        crc ^= static_cast<std::uint16_t>(data[index]) << 8;
        for (int bit = 0; bit < 8; bit++) {
            crc = (crc & 0x8000u) ? static_cast<std::uint16_t>((crc << 1) ^ 0x1021u)
                                  : static_cast<std::uint16_t>(crc << 1);
        }
    }
    return crc;
}

bool Mii::Used() const {
    if (std::all_of(raw.begin(), raw.end(), [](std::uint8_t byte) { return byte == 0; })) {
        return false;
    }
    return (Read16(raw.data() + kFlagsOffset) & kInvalidFlag) == 0;
}

std::string Mii::Name() const { return Utf16ToUtf8(raw.data() + kNameOffset, kNameLength); }

std::string Mii::Creator() const { return Utf16ToUtf8(raw.data() + kCreatorOffset, kNameLength); }

bool Mii::Female() const { return (Read16(raw.data() + kFlagsOffset) & kFemaleFlag) != 0; }

std::uint32_t Mii::Id() const {
    const std::uint8_t* at = raw.data() + kIdOffset;
    return (static_cast<std::uint32_t>(at[0]) << 24) | (static_cast<std::uint32_t>(at[1]) << 16) |
           (static_cast<std::uint32_t>(at[2]) << 8) | at[3];
}

// static
MiiDatabase MiiDatabase::Empty() {
    MiiDatabase database;
    database.mRest.assign(kMiiDatabaseSize, 0);
    std::memcpy(database.mRest.data(), "RNOD", 4);
    std::memcpy(database.mRest.data() + kMiiParadeOffset, "RNHD", 4);
    return database;
}

// static
std::optional<MiiDatabase> MiiDatabase::Parse(const std::vector<std::uint8_t>& file,
                                              bool* checksumValid) {
    if (file.size() != kMiiDatabaseSize || std::memcmp(file.data(), "RNOD", 4) != 0 ||
        std::memcmp(file.data() + kMiiParadeOffset, "RNHD", 4) != 0) {
        return std::nullopt;
    }
    if (checksumValid != nullptr) {
        const std::uint16_t stored = Read16(file.data() + kMiiCrcOffset);
        *checksumValid = stored == Crc16Ccitt(file.data(), kMiiCrcOffset);
    }

    MiiDatabase database;
    database.mRest = file;
    for (std::size_t slot = 0; slot < kMiiSlots; slot++) {
        const std::size_t offset = kEntriesOffset + slot * kMiiEntrySize;
        std::memcpy(database.mSlots[slot].raw.data(), file.data() + offset, kMiiEntrySize);
    }
    return database;
}

std::size_t MiiDatabase::Count() const {
    return static_cast<std::size_t>(
        std::count_if(mSlots.begin(), mSlots.end(), [](const Mii& mii) { return mii.Used(); }));
}

std::optional<std::size_t> MiiDatabase::FirstFree() const {
    for (std::size_t slot = 0; slot < kMiiSlots; slot++) {
        if (!mSlots[slot].Used()) {
            return slot;
        }
    }
    return std::nullopt;
}

bool MiiDatabase::Put(std::size_t slot, const Mii& mii) {
    if (slot >= kMiiSlots) {
        return false;
    }
    mSlots[slot] = mii;
    return true;
}

bool MiiDatabase::Remove(std::size_t slot) {
    if (slot >= kMiiSlots) {
        return false;
    }
    mSlots[slot] = Mii{};
    return true;
}

std::vector<std::uint8_t> MiiDatabase::Build() const {
    std::vector<std::uint8_t> file = mRest;
    file.resize(kMiiDatabaseSize, 0);
    std::memcpy(file.data(), "RNOD", 4);
    std::memcpy(file.data() + kMiiParadeOffset, "RNHD", 4);
    for (std::size_t slot = 0; slot < kMiiSlots; slot++) {
        std::memcpy(file.data() + kEntriesOffset + slot * kMiiEntrySize, mSlots[slot].raw.data(),
                    kMiiEntrySize);
    }
    const std::uint16_t crc = Crc16Ccitt(file.data(), kMiiCrcOffset);
    file[kMiiCrcOffset] = static_cast<std::uint8_t>(crc >> 8);
    file[kMiiCrcOffset + 1] = static_cast<std::uint8_t>(crc & 0xFF);
    return file;
}

}  // namespace wiinand
