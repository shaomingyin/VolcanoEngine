//
//
#ifndef VOLCANO_GAME_WORLD_HPP
#define VOLCANO_GAME_WORLD_HPP

#include <QList>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Scene.hpp>
#include <Volcano/Game/Object.hpp>

VOLCANO_GAME_BEGIN

using SceneList = QList<Scene *>;

class World: public Object {
    Q_OBJECT

public:
    World(QObject *parent = nullptr);
    ~World(void) override;

public:
    const SceneList &sceneList(void) const;
    void appendScene(Scene *scene);
    Scene *sceneAt(qsizetype index);
    void clearScenes(void);
    qsizetype sceneCount(void) const;
    void removeLastScene(void);
    void replaceScene(qsizetype index, Scene *scene);

signals:
    void sceneAdded(Volcano::Game::Scene *scene);
    void sceneRemoved(Volcano::Game::Scene *scene);

private:
    SceneList m_sceneList;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_HPP
