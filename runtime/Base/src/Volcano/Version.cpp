//
//
#include <Volcano/Version.h>

VOLCANO_BEGIN

VersionNumber::VersionNumber()
    : major_(0)
    , minor_(0)
    , patch_(0)  {
}

VersionNumber::VersionNumber(int major, int minor, int patch)
    : major_(major)
    , minor_(minor)
    , patch_(patch) {
}

std::string VersionNumber::toString() const {
    return std::string();
}

bool VersionNumber::operator==(const VersionNumber& other) const noexcept {
    return false;
}

bool VersionNumber::operator!=(const VersionNumber& other) const noexcept {
    return false;
}

bool VersionNumber::operator<(const VersionNumber& other) const noexcept {
    return false;
}

bool VersionNumber::operator<=(const VersionNumber& other) const noexcept {
    return false;
}

bool VersionNumber::operator>(const VersionNumber& other) const noexcept {
    return false;
}

bool VersionNumber::operator>=(const VersionNumber& other) const noexcept {
    return false;
}

const VersionNumber Version(VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, VOLCANO_VERSION_PATCH);

VOLCANO_END
