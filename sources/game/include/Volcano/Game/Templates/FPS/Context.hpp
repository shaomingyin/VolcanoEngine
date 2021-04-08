//
//
#ifndef VOLCANO_GAME_TEMPLATES_FPS_CONTEXT_HPP
#define VOLCANO_GAME_TEMPLATES_FPS_CONTEXT_HPP

#include <Volcano/Game/ContextBase.hpp>
#include <Volcano/Game/Templates/FPS/Common.hpp>

VOLCANO_GAME_TEMPLATES_FPS_BEGIN

class Context: public ContextBase {
public:
    Context(void);
    ~Context(void) override;
};

VOLCANO_GAME_TEMPLATES_FPS_END

#endif // VOLCANO_GAME_TEMPLATES_FPS_CONTEXT_HPP
