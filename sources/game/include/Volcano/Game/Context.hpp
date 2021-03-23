//
//
#ifndef VOLCANO_GAME_CONTEXT_HPP
#define VOLCANO_GAME_CONTEXT_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/World.hpp>

VOLCANO_GAME_BEGIN

class Context {
public:
    Context(void);
    virtual ~Context(void);

public:
    virtual bool init(void);
    virtual void update(float elapsed);

protected:
	virtual World *createWorld(void) = 0;

private:
    World *m_world;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CONTEXT_HPP
