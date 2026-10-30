#ifndef VOLCANO_GRAPHICS_LIGHT_H
#define VOLCANO_GRAPHICS_LIGHT_H

#include <cstddef>
#include <string>

#include <Eigen/Core>
#include <GL/gl.hpp>

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

// Positions and directions use world space. Direction points along emitted rays.
struct Light final {
public:
    struct Uniforms {
        void resolve(const gl::Program& program, std::size_t index);

    public:
        GLint type = -1;
        GLint radiance = -1;
        GLint position = -1;
        GLint direction = -1;
        GLint range = -1;
        GLint cone = -1;
    };

    enum class Type {
        Directional = 0,
        Point = 1,
        Spot = 2
    };

    void validate() const;

    // Fragment shader extension; the concrete pass selects its version and definitions.
    static std::string shaderSource(std::size_t maxLights);

    class Prepared final {
    public:
        explicit Prepared(const Light& light);
        void apply(const gl::Program& program, const Uniforms& uniforms) const;

    private:
        Type type_;
        Eigen::Vector3f radiance_;
        Eigen::Vector3f position_;
        Eigen::Vector3f direction_;
        float range_;
        Eigen::Vector2f cone_;
    };

public:
    Type type = Type::Directional;
    Eigen::Vector3f color = Eigen::Vector3f::Ones();
    float strength = 1.0f;
    Eigen::Vector3f position = Eigen::Vector3f::Zero();
    Eigen::Vector3f direction{0.0f, -1.0f, 0.0f};
    float range = 10.0f;
    // Cone half-angles in degrees: 0 <= inner < outer <= 90.
    float innerAngle = 15.0f;
    float outerAngle = 30.0f;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_LIGHT_H
