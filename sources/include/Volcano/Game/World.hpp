//
//
#ifndef VOLCANO_GAME_WORLD_HPP
#define VOLCANO_GAME_WORLD_HPP

#include <QList>
#include <QVector3D>
#include <QObject>
#include <QQmlListProperty>

#include <Volcano/Graphics/View.hpp>
#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Camera.hpp>
#include <Volcano/Game/Entity.hpp>

VOLCANO_GAME_BEGIN

class World: public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(Camera *mainCamera READ mainCamera WRITE setMainCamera NOTIFY mainCameraChanged)
    Q_PROPERTY(QVector3D gravity READ gravity WRITE setGravity NOTIFY gravityChanged)
    Q_PROPERTY(QQmlListProperty<Volcano::Game::Entity> entities READ qmlEntities)
    Q_CLASSINFO("DefaultProperty", "entities")

public:
    World(QObject *parent = nullptr);
    ~World(void) override;

public:
    const QString &name(void) const;
    void setName(const QString &v);
    Camera *mainCamera(void);
    void setMainCamera(Camera *cam);
    const QVector3D &gravity(void) const;
    void setGravity(const QVector3D &v);
    QQmlListProperty<Entity> qmlEntities(void);
    void update(float elapsed);
    void buildView(Graphics::View &v);

signals:
    void nameChanged(const QString &name);
    void mainCameraChanged(Camera *cam);
    void gravityChanged(const QVector3D &v);

private:
    void appendEntity(Entity *ent);
    static void qmlAppendEntity(QQmlListProperty<Entity> *list, Entity *ent);
    int entityCount(void) const;
    static int qmlEntityCount(QQmlListProperty<Entity> *list);
    Entity *entityAt(int i);
    static Entity *qmlEntityAt(QQmlListProperty<Entity> *list, int i);
    void clearEntities(void);
    static void qmlClearEntities(QQmlListProperty<Entity> *list);
    void replaceEntity(int i, Entity *ent);
    static void qmlReplaceEntity(QQmlListProperty<Entity> *list, int i, Entity *ent);
    void removeLastEntity(void);
    static void qmlRemoveLastEntity(QQmlListProperty<Entity> *list);

private:
    QString m_name;
    Camera *m_mainCamera;
    QVector3D m_gravity;
    QList<Entity *> m_entities;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_HPP
