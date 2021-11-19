//
//
#ifndef VOLCANO_EDITOR_DEBUGDRAWRENDERER_HPP
#define VOLCANO_EDITOR_DEBUGDRAWRENDERER_HPP

#define DEBUG_DRAW_EXPLICIT_CONTEXT
#include <debug_draw.hpp>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class DebugDrawRenderer: public dd::RenderInterface {
public:
    DebugDrawRenderer(void);
    ~DebugDrawRenderer(void) override;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_DEBUGDRAWRENDERER_HPP
