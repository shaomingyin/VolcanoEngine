//
//
#ifndef VOLCANO_GAME_PLAYBASE_HPP
#define VOLCANO_GAME_PLAYBASE_HPP

#include <QtPlugin>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Context.hpp>

VOLCANO_GAME_BEGIN

struct PlayBase {
    virtual bool init(Context *context) = 0;
    virtual void shutdown(void) = 0;
    virtual void update(float elapsed) = 0;
};

VOLCANO_GAME_END

#define VOLCANO_GAME_PLAY_IID "volcano.game.play"

Q_DECLARE_INTERFACE(Volcano::Game::PlayBase, VOLCANO_GAME_PLAY_IID)

#endif // VOLCANO_GAME_PLAYBASE_HPP
