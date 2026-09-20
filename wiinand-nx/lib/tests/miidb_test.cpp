// Checks the Mii database against one written by the runtime itself, the same
// way the SYSCONF test does.
//
//   miidb_test <RFL_DB.dat written by the runtime>
#include "wiinand/miidb.h"

#include <cstdio>
#include <cstring>
#include <fstream>
#include <iterator>

namespace {
int gFailures = 0;

void Check(const char* what, bool ok) {
    std::printf("  [%s] %s\n", ok ? "PASS" : "FAIL", what);
    gFailures += ok ? 0 : 1;
}

// A Mii built by hand, to test reading a used slot without needing a console:
// flags clear (valid, male), a name, and an identifier.
wiinand::Mii MakeMii(const char* name, std::uint32_t id) {
    wiinand::Mii mii;
    mii.raw.fill(0);
    for (std::size_t index = 0; name[index] != '\0' && index < 10; index++) {
        mii.raw[0x02 + index * 2] = 0;                                   // UTF-16, big endian
        mii.raw[0x03 + index * 2] = static_cast<std::uint8_t>(name[index]);
    }
    mii.raw[0x18] = static_cast<std::uint8_t>(id >> 24);
    mii.raw[0x19] = static_cast<std::uint8_t>(id >> 16);
    mii.raw[0x1A] = static_cast<std::uint8_t>(id >> 8);
    mii.raw[0x1B] = static_cast<std::uint8_t>(id);
    return mii;
}
}  // namespace

int main(int argc, char** argv) {
    using namespace wiinand;
    if (argc < 2) {
        std::printf("usage: miidb_test <RFL_DB.dat>\n");
        return 2;
    }
    std::ifstream input(argv[1], std::ios::binary);
    const std::vector<std::uint8_t> reference{std::istreambuf_iterator<char>(input),
                                              std::istreambuf_iterator<char>()};
    std::printf("wiinand Mii database test against %s (%zu bytes)\n\n", argv[1], reference.size());

    Check("the reference file is the right size", reference.size() == kMiiDatabaseSize);
    Check("Empty().Build() matches the runtime's own database byte for byte",
          MiiDatabase::Empty().Build() == reference);

    bool checksumValid = false;
    const std::optional<MiiDatabase> parsed = MiiDatabase::Parse(reference, &checksumValid);
    Check("it parses", parsed.has_value());
    if (!parsed) {
        return 1;
    }
    Check("its checksum is valid", checksumValid);
    Check("a new database holds no Miis", parsed->Count() == 0);
    Check("the first free slot is 0", parsed->FirstFree() == std::size_t{0});

    MiiDatabase database = *parsed;
    Check("add a Mii", database.Put(0, MakeMii("Luigi", 0x12345678)));
    Check("add a second", database.Put(7, MakeMii("Daisy", 0x0BADF00D)));
    Check("two Miis now", database.Count() == 2);
    Check("the first free slot moved to 1", database.FirstFree() == std::size_t{1});
    Check("names read back", database.Slots()[0].Name() == "Luigi" &&
                                 database.Slots()[7].Name() == "Daisy");
    Check("identifiers read back", database.Slots()[0].Id() == 0x12345678u);
    Check("slot 100 is refused", !database.Put(100, MakeMii("Nobody", 1)));

    const std::vector<std::uint8_t> built = database.Build();
    bool rebuiltChecksumValid = false;
    const std::optional<MiiDatabase> reparsed = MiiDatabase::Parse(built, &rebuiltChecksumValid);
    Check("it survives a round trip",
          reparsed && reparsed->Count() == 2 && reparsed->Slots()[7].Name() == "Daisy");
    Check("the checksum is recomputed", rebuiltChecksumValid);

    database.Remove(0);
    Check("remove leaves the other Mii where it was",
          database.Count() == 1 && database.Slots()[7].Name() == "Daisy");

    // A Mii whose invalid bit is set occupies a slot but is not a Mii.
    Mii disconnected = MakeMii("Ghost", 2);
    disconnected.raw[0x00] = 0x80;
    database.Put(1, disconnected);
    Check("a slot marked invalid does not count", database.Count() == 1);

    std::vector<std::uint8_t> damaged = built;
    damaged[0x100] ^= 0xFF;
    bool damagedChecksumValid = true;
    MiiDatabase::Parse(damaged, &damagedChecksumValid);
    Check("a damaged database reports a bad checksum", !damagedChecksumValid);

    std::vector<std::uint8_t> wrongMagic = built;
    wrongMagic[0] = 'X';
    Check("a file without the right magic is refused", !MiiDatabase::Parse(wrongMagic).has_value());

    std::printf("\n%s\n", gFailures ? "FAILED" : "all checks passed");
    return gFailures ? 1 : 0;
}
