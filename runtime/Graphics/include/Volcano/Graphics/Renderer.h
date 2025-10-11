//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_H
#define VOLCANO_GRAPHICS_RENDERER_H

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Renderer {
public:
    Renderer(int width, int height);
    virtual ~Renderer();

public:
    void beginFrame();
    void endFrame();
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_H
