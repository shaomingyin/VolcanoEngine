//
//
#ifndef VOLCANO_VERSION_H
#define VOLCANO_VERSION_H

#include <compare>
#include <string>
#include <string_view>
#include <tuple>

#include <Volcano/Common.h>

VOLCANO_BEGIN

class VersionNumber final {
public:
    constexpr VersionNumber() noexcept = default;

    constexpr VersionNumber(int major, int minor, int patch, std::string_view extraInfo = {})
        : major_(major)
        , minor_(minor)
        , patch_(patch)
        , extraInfo_(extraInfo) {
    }

public:
    constexpr int major() const noexcept {
        return major_;
    }

    constexpr int minor() const noexcept {
        return minor_;
    }

    constexpr int patch() const noexcept {
        return patch_;
    }

    constexpr const std::string& extraInfo() const noexcept {
        return extraInfo_;
    }

    std::string toString() const;

    // Extra information is for display and does not affect version comparisons.
    constexpr auto operator<=>(const VersionNumber& other) const noexcept {
        return std::tie(major_, minor_, patch_) <=> std::tie(other.major_, other.minor_, other.patch_);
    }

    constexpr bool operator==(const VersionNumber& other) const noexcept {
        return (*this <=> other) == 0;
    }

private:
    int major_ = 0;
    int minor_ = 0;
    int patch_ = 0;
    std::string extraInfo_;
};

extern const VersionNumber Version;

VOLCANO_END

#endif // VOLCANO_VERSION_H
