//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_H
#define VOLCANO_GRAPHICS_RENDERER_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/View.h>
#include <Volcano/Graphics/Target.h>
#include <Volcano/Graphics/Window.h>
#include <Volcano/Graphics/Texture.h>

VOLCANO_GRAPHICS_BEGIN

class Renderer {
public:
    Renderer() = default;
    virtual ~Renderer() = default;

public:
    virtual Window* createWindow(const std::string& name, int width, int height) = 0;
    virtual Texture* createTexture(int width, int height) = 0;
    virtual void render(const View& view, Duration elapsed, Target* target = nullptr) = 0;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_H
