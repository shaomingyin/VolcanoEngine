#ifndef VOLCANO_GRAPHICS_PASS_H
#define VOLCANO_GRAPHICS_PASS_H

#include <cstddef>
#include <string_view>
#include <vector>

#include <GL/gl.hpp>

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Light.h>
#include <Volcano/Graphics/Material.h>

VOLCANO_GRAPHICS_BEGIN

class Pass {
protected:
    struct ObjectUniforms final {
    public:
        void resolve(const gl::Program& program);

    public:
        GLint projection = -1;
        GLint view = -1;
        GLint model = -1;
        GLint normalMatrix = -1;
        Material::Uniforms material;
    };

    struct LightingUniforms final {
    public:
        void resolve(const gl::Program& program, std::size_t maxLights);

    public:
        GLint ambient = -1;
        GLint count = -1;
        std::vector<Light::Uniforms> lights;
    };

protected:
    Pass() = default;
    Pass(Pass&&) noexcept = default;
    Pass& operator=(Pass&&) noexcept = default;
    ~Pass() = default;

    // Source and definitions omit #version; the derived pass selects each stage and GLSL version.
    void attachShader(GLenum stage, std::string_view version, std::string_view source,
        std::string_view definitions = {});

protected:
    gl::Program program_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_PASS_H
