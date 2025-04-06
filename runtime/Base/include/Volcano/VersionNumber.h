//
//
#ifndef VOLCANO_VERSIONNUMBER_H
#define VOLCANO_VERSIONNUMBER_H

#include <string>

#include <Volcano/Common.h>

VOLCANO_BEGIN

class VersionNumber final {
public:
    VersionNumber()
        : major_number_(0)
        , minor_number_(0)
        , patch_number_(0) {
    }

    VersionNumber(const VersionNumber& other)
        : major_number_(other.major_number_)
        , minor_number_(other.minor_number_)
        , patch_number_(other.patch_number_) {
    }

    VersionNumber(int major_number, int minor_number, int patch_number)
        : major_number_(major_number)
        , minor_number_(minor_number)
        , patch_number_(patch_number) {
    }

public:
    int majorNumber() const {
        return major_number_;
    }

    int minorNumber() const {
        return minor_number_;
    }

    int patchNumber() const {
        return patch_number_;
    }

    void reset(int major_number = 0, int minor_number = 0, int patch_number = 0) {
        major_number_ = major_number;
        minor_number_ = minor_number;
        patch_number_ = patch_number;
    }

    VersionNumber& operator=(const VersionNumber& other) {
        major_number_ = other.major_number_;
        minor_number_ = other.minor_number_;
        patch_number_ = other.patch_number_;
        return (*this);
    }

    std::string toString() const {
        return fmt::format("{}.{}.{}", major_number_, minor_number_, patch_number_);
    }

    bool operator==(const VersionNumber& other) const {
        return (major_number_ == other.major_number_ &&
                minor_number_ == other.minor_number_ &&
                patch_number_ == other.patch_number_);
    }

    bool operator!=(const VersionNumber& other) const {
        return !(operator==(other));
    }

    bool operator<(const VersionNumber& other) const {
        return (major_number_ < other.major_number_ ||
                minor_number_ < other.minor_number_ ||
                patch_number_ < other.patch_number_);
    }

    bool operator<=(const VersionNumber& other) const {
        return (major_number_ <= other.major_number_ ||
                minor_number_ <= other.minor_number_ ||
                patch_number_ <= other.patch_number_);
    }

    bool operator>(const VersionNumber& other) const {
        return !(operator<=(other));
    }

    bool operator>=(const VersionNumber& other) const {
        return !(operator<(other));
    }

private:
    int major_number_;
    int minor_number_;
    int patch_number_;
};

extern const VersionNumber Version;

VOLCANO_END

#endif // VOLCANO_VERSIONNUMBER_H
