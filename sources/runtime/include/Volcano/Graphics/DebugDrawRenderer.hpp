//
//
#ifndef VOLCANO_GRAPHICS_DEBUGDRAWRENDERER_HPP
#define VOLCANO_GRAPHICS_DEBUGDRAWRENDERER_HPP

#define DEBUG_DRAW_EXPLICIT_CONTEXT
#include <debug_draw.hpp>

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class DebugDrawRenderer: public dd::RenderInterface {
public:
    DebugDrawRenderer(void);
    ~DebugDrawRenderer(void) override;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_DEBUGDRAWRENDERER_HPP
