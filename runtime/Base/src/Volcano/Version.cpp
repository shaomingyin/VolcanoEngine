//
//
#include <fmt/format.h>

#include <Volcano/Version.h>

VOLCANO_BEGIN

std::string VersionNumber::toString() const {
    if (extraInfo_.empty()) {
        return fmt::format("{}.{}.{}", major_, minor_, patch_);
    }

    return fmt::format("{}.{}.{}-{}", major_, minor_, patch_, extraInfo_);
}

constinit const VersionNumber Version(VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, VOLCANO_VERSION_PATCH);

VOLCANO_END
