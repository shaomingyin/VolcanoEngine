//
//
#ifndef VOLCANO_GAME_WORLD_HPP
#define VOLCANO_GAME_WORLD_HPP

#include <QObject>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Scene.hpp>

VOLCANO_GAME_BEGIN

class World: public QObject {
    Q_OBJECT

public:
    World(QObject *parent = nullptr);
    ~World(void) override;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_HPP
