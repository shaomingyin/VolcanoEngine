//
//
#ifndef VOLCANO_GAME_LIGHT_HPP
#define VOLCANO_GAME_LIGHT_HPP

#include <Volcano/Math.hpp>
#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class Light {
public:
    Light();
    Light(const Eigen::Vector4f& color);

public:
    Light& operator=(const Light& other) {
        color_ = other.color_;
        return (*this);
    }

    const Eigen::Vector4f& color() const noexcept {
        return color_;
    }

    void setColor(const Eigen::Vector4f& v) noexcept {
        color_ = v;
    }

private:
    Eigen::Vector4f color_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_LIGHT_HPP
