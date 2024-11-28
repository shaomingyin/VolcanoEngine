//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_H
#define VOLCANO_GRAPHICS_RENDERER_H

#include <array>
#include <bitset>

#include <Volcano/Color.h>
#include <Volcano/World/Scene.h>
#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/View.h>
#include <Volcano/Graphics/Pipeline.h>

VOLCANO_GRAPHICS_BEGIN

class Renderer {
public:
    Renderer(World::Scene& scene);
    virtual ~Renderer();

public:
    void update(Duration elapsed);
    void render();

private:
    World::Scene& scene_;
    //Pipeline vector_graphics_pipeline_;
    //Pipeline gbuffer_pipeline_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_H
