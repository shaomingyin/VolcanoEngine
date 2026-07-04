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

void to_json(nlohmann::json& json, const VersionNumber& v) {
    json = nlohmann::json::object();
    json["major"] = v.major();
    json["minor"] = v.minor();
    json["patch"] = v.patch();
}

void from_json(const nlohmann::json& json, VersionNumber& v) {
    if (json.is_object() && json.contains("major") && json.contains("minor") && json.contains("patch")) {
        v.major_ = json["major"].get<int>();
        v.minor_ = json["minor"].get<int>();
        v.patch_ = json["patch"].get<int>();
    } else {
        throw std::invalid_argument("Invalid JSON format for VersionNumber");
    }
}

const VersionNumber Version(VOLCANO_VERSION_MAJOR, VOLCANO_VERSION_MINOR, VOLCANO_VERSION_PATCH);

VOLCANO_END
