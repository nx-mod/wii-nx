// Checks the library against a SYSCONF written by the runtime itself, so the
// two cannot drift: a NAND made by either has to satisfy the other.
//
//   sysconf_test <SYSCONF written by the runtime>
#include "wiinand/sysconf.h"

#include <cstdio>
#include <fstream>
#include <iterator>

namespace {
int gFailures = 0;

void Check(const char* what, bool ok) {
    std::printf("  [%s] %s\n", ok ? "PASS" : "FAIL", what);
    gFailures += ok ? 0 : 1;
}
}  // namespace

int main(int argc, char** argv) {
    using namespace wiinand;
    if (argc < 2) {
        std::printf("usage: sysconf_test <SYSCONF>\n");
        return 2;
    }
    std::ifstream input(argv[1], std::ios::binary);
    const std::vector<std::uint8_t> reference{std::istreambuf_iterator<char>(input),
                                              std::istreambuf_iterator<char>()};
    std::printf("wiinand sysconf test against %s (%zu bytes)\n\n", argv[1], reference.size());

    Check("the reference file is 16 KiB", reference.size() == kSysconfSize);

    const std::vector<std::uint8_t> built = Sysconf::Defaults().Build();
    Check("Defaults().Build() matches the runtime's own SYSCONF byte for byte", built == reference);

    const std::optional<Sysconf> parsed = Sysconf::Parse(reference);
    Check("it parses", parsed.has_value());
    if (!parsed) {
        return 1;
    }
    Check("14 settings", parsed->Items().size() == 14);
    Check("IPL.LNG is English (1)", parsed->Number(sysconf_keys::kLanguage) == 1u);
    Check("IPL.AR is 16:9 (1)", parsed->Number(sysconf_keys::kAspectRatio) == 1u);
    Check("BT.MOT (rumble) is on", parsed->Boolean(sysconf_keys::kRumble));
    Check("BT.SENS is a long holding 3", parsed->Number(sysconf_keys::kSensorBarSensitivity) == 3u);
    Check("IPL.SADR is a 0x1007-byte array",
          parsed->Find(sysconf_keys::kSimpleAddress) != nullptr &&
              parsed->Find(sysconf_keys::kSimpleAddress)->data.size() == 0x1007);
    Check("an unknown setting is absent", !parsed->Number("NO.SUCH").has_value());

    Check("parse then build is unchanged", parsed->Build() == reference);

    Sysconf edited = *parsed;
    Check("set IPL.AR to 4:3", edited.SetNumber(sysconf_keys::kAspectRatio, 0));
    Check("set IPL.E60 off", edited.SetBoolean(sysconf_keys::kPal60, false));
    const std::vector<std::uint8_t> editedFile = edited.Build();
    const std::optional<Sysconf> reparsed = Sysconf::Parse(editedFile);
    Check("the edit survives a round trip",
          reparsed && reparsed->Number(sysconf_keys::kAspectRatio) == 0u &&
              !reparsed->Boolean(sysconf_keys::kPal60, true));

    std::size_t differing = 0;
    for (std::size_t index = 0; index < reference.size(); index++) {
        differing += editedFile[index] != reference[index] ? 1 : 0;
    }
    Check("two edits changed exactly two bytes", differing == 2);

    Check("an array cannot be set as a number", !edited.SetNumber(sysconf_keys::kSimpleAddress, 1));

    std::printf("\n%s\n", gFailures ? "FAILED" : "all checks passed");
    return gFailures ? 1 : 0;
}
