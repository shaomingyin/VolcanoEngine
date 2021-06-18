//
//
#ifndef VOLCANO_WORLD_HPP
#define VOLCANO_WORLD_HPP

#include <QList>
#include <QObject>
#include <QQmlListProperty>

#include <Volcano/Common.hpp>
#include <Volcano/Object.hpp>

VOLCANO_BEGIN

class World: public QObject {
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Object> objects READ qmlObjects)
    Q_CLASSINFO("DefaultProperty", "objects")

public:
    World(QObject *parent = nullptr);
    ~World(void) override;

public:
    virtual void update(float elapsed);
    const QList<Object *> &objects(void) const;
    QQmlListProperty<Object> qmlObjects(void);

signals:
    void objectAdded(Object *object);
    void objectRemoved(Object *object);

private:
    static void appendObject(QQmlListProperty<Object> *list, Object *object);
    static qsizetype objectCount(QQmlListProperty<Object> *list);
    static Object *objectAt(QQmlListProperty<Object> *list, qsizetype index);
    static void clearObjects(QQmlListProperty<Object> *list);
    static void replaceObject(QQmlListProperty<Object> *list, qsizetype index, Object *object);
    static void removeLastObject(QQmlListProperty<Object> *list);

private:
    QList<Object *> m_objects;
};

VOLCANO_END

#endif // VOLCANO_WORLD_HPP
