//
//
#ifndef VOLCANO_GRAPHICS_DIRECTIONALLIGHT_HPP
#define VOLCANO_GRAPHICS_DIRECTIONALLIGHT_HPP

#include <Volcano/Math.hpp>
#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Light.hpp>

VOLCANO_GRAPHICS_BEGIN

class DirectionalLight: public Light {
    RTTR_ENABLE(Light)
    
public:
    DirectionalLight()
        : direction_(0.0f, -1.0f, 0.0f) {
    }

    DirectionalLight(const DirectionalLight& other)
        : Light(other)
        , direction_(other.direction_) {
    }

    DirectionalLight(const Eigen::Vector4f& color, const Eigen::Vector3f& direction)
        : Light(color)
        , direction_(direction) {
    }

public:
    DirectionalLight& operator=(const DirectionalLight& other) = default;

    const Eigen::Vector3f& direction() const noexcept {
        return direction_;
    }

    void setDirection(const Eigen::Vector3f& v) noexcept {
        direction_ = v;
    }

private:
    Eigen::Vector3f direction_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_DIRECTIONALLIGHT_HPP
