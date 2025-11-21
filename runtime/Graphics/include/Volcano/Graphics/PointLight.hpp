//
//
#ifndef VOLCANO_GRAPHICS_POINTLIGHT_HPP
#define VOLCANO_GRAPHICS_POINTLIGHT_HPP

#include <Volcano/Math.hpp>
#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Light.hpp>

VOLCANO_GRAPHICS_BEGIN

class PointLight: public Light {
    RTTR_ENABLE(Light)
    
public:
    PointLight() {
    }

    PointLight(const PointLight& other)
        : Light(other) {
    }

    PointLight(const Eigen::Vector4f& color)
        : Light(color) {
    }

public:
    PointLight& operator=(const PointLight& other) = default;

private:
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_POINTLIGHT_HPP
