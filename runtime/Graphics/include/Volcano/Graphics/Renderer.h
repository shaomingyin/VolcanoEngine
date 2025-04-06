//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_H
#define VOLCANO_GRAPHICS_RENDERER_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/View.h>
#include <Volcano/Graphics/Context.h>

VOLCANO_GRAPHICS_BEGIN

class Renderer: public Context {
public:
    Renderer();
    ~Renderer() override;

public:
    void render(const View& view, Duration elapsed);

private:
    GL3WProcs gl3w_;
	NVGcontext* nvg_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_H
