//
//
#ifndef VOLCANO_GAME_CONTEXTBASE_HPP
#define VOLCANO_GAME_CONTEXTBASE_HPP

#include <string>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/World.hpp>
#include <Volcano/Game/Camera.hpp>
#include <Volcano/Game/Entity.hpp>

VOLCANO_GAME_BEGIN

class ContextBase {
public:
    ContextBase(void);
    virtual ~ContextBase(void);

public:
    virtual bool init(void);
    virtual void frame(float elapsed);
    virtual std::string name(void);
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CONTEXTBASE_HPP
