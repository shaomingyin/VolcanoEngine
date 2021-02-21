//
//
#ifndef GAMEPLAY_HPP
#define GAMEPLAY_HPP

#include <QObject>

#include <Volcano/Game/Play.hpp>

class GamePlay: public Volcano::Game::Play {
    Q_OBJECT

public:
    GamePlay(QObject *parent = nullptr);
    ~GamePlay(void) override;

public:
    bool init(Volcano::Game::Context *context) override;
    void shutdown(void) override;
    void update(float elapsed) override;
};

#endif // GAMEPLAY_HPP
