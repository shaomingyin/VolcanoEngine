//
//
#ifndef VOLCANO_GAME_CONTEXT_H
#define VOLCANO_GAME_CONTEXT_H

#include <Volcano/Game/Common.h>

VOLCANO_GAME_BEGIN

class Context {
public:
    Context();
    virtual ~Context();

public:
    entt::registry& registry() {
        return registry_;
    }

    const entt::registry& registry() const {
        return registry_;
    }

protected:
    

private:
    entt::registry registry_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CONTEXT_H
