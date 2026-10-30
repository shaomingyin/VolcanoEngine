#ifndef VOLCANO_GRAPHICS_MATERIAL_H
#define VOLCANO_GRAPHICS_MATERIAL_H

#include <GL/gl.hpp>

#include <Volcano/Color.h>
#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Texture.h>

VOLCANO_GRAPHICS_BEGIN

struct Material final {
    struct Uniforms {
        void resolve(const gl::Program& program);

    public:
        GLint color = -1;
        GLint textured = -1;
        GLint lit = -1;
        GLint texture = -1;
    };

    void apply(const gl::Program& program, const Uniforms& uniforms) const;

public:
    Colorf color = Colorf::White;
    const Texture* texture = nullptr;
    bool lit = false;
};

VOLCANO_GRAPHICS_END

#endif
