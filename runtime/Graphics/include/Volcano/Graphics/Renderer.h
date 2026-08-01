//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_H
#define VOLCANO_GRAPHICS_RENDERER_H

#include <atomic>

#include <Volcano/Math.h>
#include <Volcano/World/Scene.h>
#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Camera.h>
#include <Volcano/Graphics/Frame.h>

VOLCANO_GRAPHICS_BEGIN

class Renderer {
public:
    Renderer(World::Scene& scene);
    virtual ~Renderer();

public:
    void build() noexcept;
    void render() const noexcept;

private:
    World::Scene& scene_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_H
