//
//
#ifndef VOLCANO_GAME_SCENE_HPP
#define VOLCANO_GAME_SCENE_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/DirectionalLight.hpp>

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
