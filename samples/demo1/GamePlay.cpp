//
//
#include "GamePlay.hpp"

GamePlay::GamePlay(QObject *parent):
    Volcano::Game::Play(parent)
{
}

GamePlay::~GamePlay(void)
{
}

bool GamePlay::init(Volcano::Game::Context *context)
{
    if (!Volcano::Game::Play::init(context))
        return false;

    return true;
}

void GamePlay::shutdown(void)
{
    Volcano::Game::Play::shutdown();
}

void GamePlay::update(float elapsed)
{
    Volcano::Game::Play::update(elapsed);
}
