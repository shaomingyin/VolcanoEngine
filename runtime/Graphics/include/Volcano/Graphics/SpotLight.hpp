//
//
#ifndef VOLCANO_GRAPHICS_SPOTLIGHT_HPP
#define VOLCANO_GRAPHICS_SPOTLIGHT_HPP

#include <Volcano/Math.hpp>
#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/DirectionalLight.hpp>

VOLCANO_GRAPHICS_BEGIN

class SpotLight: public DirectionalLight {
    RTTR_ENABLE(DirectionalLight)
    
public:
    SpotLight() {
    }

    SpotLight(const SpotLight& other) = default;

    SpotLight(const Eigen::Vector4f& color, const Eigen::Vector3f& direction)
        : DirectionalLight(color, direction) {
    }

public:
    PointLight& operator=(const PointLight& other) = default;

private:
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_SPOTLIGHT_HPP
