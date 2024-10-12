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
    virtual bool init(GL3WGetProcAddressProc gl_address_proc, int width, int height);

public:
    bool begin();
    void end();
    void update(std::chrono::steady_clock::duration elapsed);

private:
    GL3WProcs gl3w_;
    int widht_;
    int height_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_H
