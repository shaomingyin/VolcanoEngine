//
//
#include <Volcano/ScopeGuard.h>
#include <Volcano/OpenGL/Texture.h>

VOLCANO_OPENGL_BEGIN

Texture::Texture(int width, int height)
    : Graphics::Texture(width, height) {
}

Texture::~Texture() {
}

VOLCANO_OPENGL_END
