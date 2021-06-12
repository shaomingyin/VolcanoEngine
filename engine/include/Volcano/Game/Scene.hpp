//
//
#ifndef VOLCANO_GAME_SCENE_HPP
#define VOLCANO_GAME_SCENE_HPP

#include <QList>
#include <QVector3D>
#include <QQmlListProperty>

#include <bullet/btBulletDynamicsCommon.h>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Object.hpp>

VOLCANO_GAME_BEGIN

class Scene: public Object {
    Q_OBJECT
    Q_PROPERTY(bool dynamic READ isDynamic WRITE setDynamic NOTIFY dynamicChanged)
    Q_PROPERTY(QVector3D gravity READ gravity WRITE setGravity NOTIFY gravityChanged)
    Q_PROPERTY(QQmlListProperty<Object> objects READ objects)
    Q_CLASSINFO("DefaultProperty", "objects")

public:
    Scene(QObject *parent = nullptr);
    ~Scene(void) override;

public:
    bool isDynamic(void) const;
    void setDynamic(bool v);
    const QVector3D &gravity(void) const;
    void setGravity(const QVector3D &v);
    QQmlListProperty<Object> objects(void);

signals:
    void dynamicChanged(bool v);
    void gravityChanged(const QVector3D &v);

private:
    static void appendObject(QQmlListProperty<Object> *list, Object *object);
    static qsizetype objectCount(QQmlListProperty<Object> *list);
    static Object *objectAt(QQmlListProperty<Object> *list, qsizetype index);
    static void clearObjects(QQmlListProperty<Object> *list);
    static void replaceObject(QQmlListProperty<Object> *list, qsizetype index, Object *object);
    static void removeLastObject(QQmlListProperty<Object> *list);

private:
    btDynamicsWorld *m_btWorld;
    QVector3D m_gravity;
    QList<Object *> m_objects;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SCENE_HPP
