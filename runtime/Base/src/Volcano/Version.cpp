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
    return std::format("{}.{}.{}", major_, minor_, patch_);
}

bool VersionNumber::operator==(const VersionNumber& other) const noexcept {
    return
        major_ == other.major_ &&
        minor_ == other.minor_ &&
        patch_ == other.patch_;
}

bool VersionNumber::operator!=(const VersionNumber& other) const noexcept {
    return !operator==(other);
}

bool VersionNumber::operator<(const VersionNumber& other) const noexcept {
    if (major_ < other.major_) {
        return true;
    }
    if (major_ > other.major_) {
        return false;
    }
    if (minor_ < other.minor_) {
        return true;
    }
    if (minor_ > other.minor_) {
        return false;
    }
    return (patch_ < other.patch_);
}

bool VersionNumber::operator<=(const VersionNumber& other) const noexcept {
    return !operator>(other);
}

bool VersionNumber::operator>(const VersionNumber& other) const noexcept {
    if (major_ > other.major_) {
        return true;
    }
    if (major_ < other.major_) {
        return false;
    }
    if (minor_ > other.minor_) {
        return true;
    }
    if (minor_ < other.minor_) {
        return false;
    }
    return (patch_ > other.patch_);
}

bool VersionNumber::operator>=(const VersionNumber& other) const noexcept {
    return !operator<(other);
}

const VersionNumber Version(VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, VOLCANO_VERSION_PATCH);

VOLCANO_END
