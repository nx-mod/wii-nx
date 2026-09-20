#pragma once

// RFL_DB.dat: the Wii's Mii database.
//
// One file at /shared2/menu/FaceLib/RFL_DB.dat holding up to 100 Miis, which
// every game reads - the racers in Mario Kart, the players in Wii Sports. wii-nx
// keeps one shared copy, so a Mii made once appears everywhere.
//
// Layout: "RNOD", then 100 slots of 74 bytes; "RNHD" at 0x1D00 begins the Mii
// Parade section; a CRC-16/CCITT over everything before it sits at 0x1F1DE.
//
// A slot's 74 bytes are the console's own Mii structure. This reads the parts
// needed to show and move Miis about - whether the slot is used, the Mii's name
// and its creator - and carries the rest untouched, so a Mii copied through here
// is bit-identical to the one that went in.

#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace wiinand {

inline constexpr std::size_t kMiiDatabaseSize = 0x1F1E0;
inline constexpr std::size_t kMiiEntrySize = 0x4A;
inline constexpr std::size_t kMiiSlots = 100;
inline constexpr std::size_t kMiiParadeOffset = 0x1D00;
inline constexpr std::size_t kMiiCrcOffset = 0x1F1DE;

// CRC-16/CCITT (polynomial 0x1021, zero initial value), stored big-endian.
std::uint16_t Crc16Ccitt(const std::uint8_t* data, std::size_t length);

// One Mii, as its 74 bytes plus the fields worth showing.
struct Mii {
    std::array<std::uint8_t, kMiiEntrySize> raw{};

    bool Used() const;
    std::string Name() const;           // UTF-8, converted from the file's UTF-16
    std::string Creator() const;
    bool Female() const;
    std::uint32_t Id() const;           // the Mii's own identifier
};

class MiiDatabase {
  public:
    // An empty database, the same one the runtime writes for a new NAND.
    static MiiDatabase Empty();

    // Parses a file. Returns nothing when it is the wrong size or its magics
    // are missing; `checksumValid` reports the CRC separately, since a database
    // with a stale checksum is still readable.
    static std::optional<MiiDatabase> Parse(const std::vector<std::uint8_t>& file,
                                            bool* checksumValid = nullptr);

    const std::array<Mii, kMiiSlots>& Slots() const { return mSlots; }
    std::size_t Count() const;                       // how many slots are used
    std::optional<std::size_t> FirstFree() const;

    // Places a Mii in a slot, or removes one. Both renumber nothing: slots are
    // positions on the console's Mii page, and people expect them to stay put.
    bool Put(std::size_t slot, const Mii& mii);
    bool Remove(std::size_t slot);

    // The file, with its checksum recomputed.
    std::vector<std::uint8_t> Build() const;

  private:
    std::array<Mii, kMiiSlots> mSlots{};
    // Everything this class does not interpret (the Mii Parade section and the
    // rest) is carried through untouched.
    std::vector<std::uint8_t> mRest;
};

}  // namespace wiinand
