//
//
#ifndef VOLCANO_GAME_SCENE_HPP
#define VOLCANO_GAME_SCENE_HPP

#include <QList>
#include <QObject>

//#define register
//#include <bullet/btBulletDynamicsCommon.h>
//#undef register

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Camera.hpp>
#include <Volcano/Game/Entity.hpp>

VOLCANO_GAME_BEGIN

using EntityList = QList<Entity *>;

class VOLCANO_GAME_API Scene: public QObject {
    Q_OBJECT

public:
    Scene(QObject *parent = nullptr);
    virtual ~Scene(void);

public:
    bool init(void);
    void shutdown(void);
    void update(float elapsed);

private:
    //btSimpleDynamicsWorld *m_dynamicWorld;
    EntityList m_entityList;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SCENE_HPP
