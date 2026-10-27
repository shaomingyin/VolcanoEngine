#include <GL/gl.hpp>

#include <Volcano/Graphics/Material.h>
#include <Volcano/Graphics/Texture.h>

VOLCANO_GRAPHICS_BEGIN

void Material::Uniforms::resolve(const gl::Program& program) {
    color = program.uniformLocation("uColor", GL_FLOAT_VEC4);
    textured = program.uniformLocation("uTextured", GL_BOOL);
    lit = program.uniformLocation("uLit", GL_BOOL);
    texture = program.uniformLocation("uTexture", GL_SAMPLER_2D);
}

void Material::apply(const gl::Program& program, const Uniforms& uniforms) const {
    program.uniform(uniforms.lit, lit ? 1 : 0);
    program.uniform4f(uniforms.color, color.red(), color.green(), color.blue(), color.alpha());
    program.uniform(uniforms.textured, texture != nullptr ? 1 : 0);
    program.uniform(uniforms.texture, static_cast<GLint>(TextureUnit));

    glActiveTexture(GL_TEXTURE0 + TextureUnit);
    glBindSampler(TextureUnit, 0);
    glBindTexture(GL_TEXTURE_2D, texture != nullptr ? texture->texture().id() : 0);
}

VOLCANO_GRAPHICS_END
