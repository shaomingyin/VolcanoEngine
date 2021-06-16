//
//
#ifndef VOLCANO_WORLD_HPP
#define VOLCANO_WORLD_HPP

#include <QList>
#include <QObject>
#include <QQmlListProperty>

#include <bullet/btBulletDynamicsCommon.h>

#include <Volcano/Common.hpp>
#include <Volcano/Object.hpp>

VOLCANO_BEGIN

class World: public QObject {
    Q_OBJECT
    Q_PROPERTY(bool dynamic READ isDynamic WRITE setDynamic NOTIFY dynamicChanged)
    Q_PROPERTY(QQmlListProperty<Object> objects READ qmlObjects)
    Q_CLASSINFO("DefaultProperty", "objects")

public:
    World(QObject *parent = nullptr);
    ~World(void) override;

public:
    bool isDynamic(void) const;
    void setDynamic(bool v);
    const QList<Object *> &objects(void) const;
    QQmlListProperty<Object> qmlObjects(void);

signals:
    void dynamicChanged(bool v);
    void objectAdded(Object *object);
    void objectRemoved(Object *object);

private:
    void initDynamicWorld(void);
    void shutdownDynamicWorld(void);
    static void appendObject(QQmlListProperty<Object> *list, Object *object);
    static qsizetype objectCount(QQmlListProperty<Object> *list);
    static Object *objectAt(QQmlListProperty<Object> *list, qsizetype index);
    static void clearObjects(QQmlListProperty<Object> *list);
    static void replaceObject(QQmlListProperty<Object> *list, qsizetype index, Object *object);
    static void removeLastObject(QQmlListProperty<Object> *list);

private:
    btDynamicsWorld *m_btWorld;
    QList<Object *> m_objects;
};

VOLCANO_END

#endif // VOLCANO_WORLD_HPP
