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
    virtual bool init(int width, int height);

public:
    bool begin();
    void end();
    void update(Duration elapsed);

private:
    GL3WProcs gl3w_;
    int widht_;
    int height_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_H
