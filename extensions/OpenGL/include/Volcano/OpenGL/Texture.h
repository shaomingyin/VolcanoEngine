//
//
#ifndef VOLCANO_OPENGL_TEXTURE_H
#define VOLCANO_OPENGL_TEXTURE_H

#include <Volcano/Graphics/Texture.h>
#include <Volcano/OpenGL/Common.h>

VOLCANO_OPENGL_BEGIN

class Texture: public Graphics::Texture {
public:
    Texture(int width, int height);
    ~Texture() override;

public:

private:
};

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_TEXTURE_H
