//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_H
#define VOLCANO_GRAPHICS_RENDERER_H

#include <array>
#include <bitset>

#include <Volcano/Color.h>
#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/View.h>
#include <Volcano/Graphics/Pipeline.h>

VOLCANO_GRAPHICS_BEGIN

class Renderer {
public:
    Renderer(entt::registry& registry);
    virtual ~Renderer();

public:
    void frame(Duration elapsed);

private:
    void buildView(View& view) const;

private:
    entt::registry& registry_;
    Pipeline vector_graphics_pipeline_;
    Pipeline gbuffer_pipeline_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_H
