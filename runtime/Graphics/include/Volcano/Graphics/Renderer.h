//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_H
#define VOLCANO_GRAPHICS_RENDERER_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/View.h>

VOLCANO_GRAPHICS_BEGIN

class Renderer {
public:
    Renderer();
    virtual ~Renderer();

public:
    virtual void init(GL3WGetProcAddressProc get_proc, int width, int height);

public:
    void render(const View& view, Duration elapsed);

private:
    GL3WProcs gl3w_;
    int widht_;
    int height_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_H
