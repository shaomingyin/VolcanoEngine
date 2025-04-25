/*
 *
 */
#ifndef VOLCANO_FRAMEWORK_GAME_H
#define VOLCANO_FRAMEWORK_GAME_H

#include <Volcano/Framework/Common.h>
#include <Volcano/Framework/Context.h>

VOLCANO_FRAMEWORK_BEGIN

class Game {
public:
    Game(Context& context);

public:
    Context& context() {
        return context_;
    }

    const Context& context() const {
        return context_;
    }

private:
    Context& context_;
};

VOLCANO_FRAMEWORK_END

#endif /* VOLCANO_FRAMEWORK_GAME_H */
