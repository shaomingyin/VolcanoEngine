//
//
#ifndef VOLCANO_VERSION_H
#define VOLCANO_VERSION_H

#include <Volcano/Common.h>

VOLCANO_BEGIN

class VersionNumber final {
public:
    VersionNumber();
    VersionNumber(int major, int minor, int patch);

public:
    int major() const noexcept {
        return major_;
    }

    int minor() const noexcept {
        return minor_;
    }

    int patch() const noexcept {
        return patch_;
    }

    sf::String toString() const;

    bool operator==(const VersionNumber& other) const noexcept;
    bool operator!=(const VersionNumber& other) const noexcept;
    bool operator<(const VersionNumber& other) const noexcept;
    bool operator<=(const VersionNumber& other) const noexcept;
    bool operator>(const VersionNumber& other) const noexcept;
    bool operator>=(const VersionNumber& other) const noexcept;

private:
    int major_;
    int minor_;
    int patch_;
};

extern const VersionNumber Version;

VOLCANO_END

#endif // VOLCANO_VERSION_H
