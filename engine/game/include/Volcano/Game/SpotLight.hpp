//
//
#ifndef VOLCANO_GAME_SCENE_HPP
#define VOLCANO_GAME_SCENE_HPP

#include <QObject>

#include <bullet/btBulletDynamicsCommon.h>

#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class Scene: public QObject {
    Q_OBJECT

public:
    Scene(QObject *parent = nullptr);
    ~Scene(void) override;

private:
    btDynamicsWorld *m_btWorld;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SCENE_HPP
