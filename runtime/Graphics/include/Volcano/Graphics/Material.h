#ifndef VOLCANO_GRAPHICS_MATERIAL_H
#define VOLCANO_GRAPHICS_MATERIAL_H

#include <GL/gl3w.h>

#include <Volcano/Graphics/Common.h>
#include <Volcano/Color.h>

namespace gl {
class Program;
}

VOLCANO_GRAPHICS_BEGIN

class Texture;

struct Material final {
public:
    struct Uniforms final {
    public:
        void resolve(const gl::Program& program);

    public:
        GLint color = -1;
        GLint textured = -1;
        GLint lit = -1;
        GLint texture = -1;
    };

public:
    // The caller must bind program; uniforms must have been resolved for it.
    void apply(const gl::Program& program, const Uniforms& uniforms) const;

public:
    Colorf color = Colorf::White;
    const Texture* texture = nullptr;
    bool lit = false;

private:
    static constexpr GLuint TextureUnit = 0;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_MATERIAL_H
