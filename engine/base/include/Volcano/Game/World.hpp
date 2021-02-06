//
//
#ifndef VOLCANO_GAME_WORLD_HPP
#define VOLCANO_GAME_WORLD_HPP

#include <QList>
#include <QObject>

//#define register
//#include <bullet/btBulletDynamicsCommon.h>
//#undef register

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Camera.hpp>
#include <Volcano/Game/Entity.hpp>

VOLCANO_GAME_BEGIN

using Cameras = QList<Camera *>;
using Entities = QList<Entity *>;

class VOLCANO_API World: public QObject
{
    Q_OBJECT

public:
    World(QObject *parent = nullptr);
    virtual ~World(void);

public:
    bool init(void);
    void shutdown(void);
    void update(float elapsed);

private:
    //btSimpleDynamicsWorld *m_dynamicWorld;
    Cameras m_cameras;
    Entities m_entities;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_HPP
