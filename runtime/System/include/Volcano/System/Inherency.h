//
//
#ifndef VOLCANO_WORLD_INHERENCY_H
#define VOLCANO_WORLD_INHERENCY_H

#include <string>

#include <Volcano/Error.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Inherency {
public:
    Inherency(std::string name, Eigen::Affine3f transform)
        : flags_(FlagEnabled | FlagVisible)
        , name_(std::move(name))
        , transform_(transform) {
    }

    Inherency(const Inherency& other)
        : flags_(other.flags_)
        , name_(other.name_)
        , transform_(other.transform_) {
    }

    Inherency(Inherency&& other)
        : flags_(other.flags_)
        , name_(std::move(other.name_))
        , transform_(other.transform_) {
    }

public:
    bool isEnabled() const {
        return flags_ & FlagEnabled;
    }

    void enable() {
        flags_ |= FlagEnabled;
    }

    void disable() {
        flags_ &= ~FlagEnabled;
    }

    bool isVisible() const {
        return flags_ & FlagVisible;
    }

    void show() {
        flags_ |= FlagVisible;
    }

    void hide() {
        flags_ &= ~FlagVisible;
    }

    std::string& name() {
        return name_;
    }

    const std::string& name() const {
        return name_;
    }

    Eigen::Affine3f& transform() {
        return transform_;
    }

    const Eigen::Affine3f& transform() const {
        return transform_;
    }

    Inherency& operator=(const Inherency& other) {
        if (this != &other) {
            flags_ = other.flags_;
            name_ = other.name_;
            transform_ = other.transform_;
        }
        return (*this);
    }

    Inherency& operator=(Inherency&& other) {
        if (this != &other) {
            flags_ = other.flags_;
            name_ = std::move(other.name_);
            transform_ = other.transform_;
        }
        return (*this);
    }

private:
    enum {
        FlagEnabled = 0x1,
        FlagVisible = 0x2
    };

private:
    int flags_;
    std::string name_;
    Eigen::Affine3f transform_;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_INHERENCY_H
