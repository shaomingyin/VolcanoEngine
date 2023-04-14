//
//
#ifndef VOLCANO_GAME_SCENE_HPP
#define VOLCANO_GAME_SCENE_HPP

#include <QMap>
#include <QString>
#include <QObject>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Entity.hpp>
#include <Volcano/Game/Object.hpp>

VOLCANO_GAME_BEGIN

using EntityList = QList<Entity *>;

class Scene: public Object {
    Q_OBJECT

public:
    Scene(QObject *parent = nullptr);
    ~Scene(void) override;

public:
    const EntityList &entityList(void) const;
    void appendEntity(Entity *entity);
    Entity *entityAt(qsizetype index);
    void clearEntities(void);
    qsizetype entityCount(void) const;
    void removeLastEntity(void);
    void replaceEntity(qsizetype index, Entity *entity);

signals:
    void entityAdded(Volcano::Game::Entity *entity);
    void entityRemoved(Volcano::Game::Entity *entity);

private:
    EntityList m_entityList;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SCENE_HPP
