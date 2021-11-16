//
//
#ifndef VOLCANO_GAME_WORLD_HPP
#define VOLCANO_GAME_WORLD_HPP

#include <QList>
#include <QString>
#include <QObject>
#include <QElapsedTimer>
#include <QQmlListProperty>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Object.hpp>
#include <Volcano/Game/Light.hpp>

VOLCANO_GAME_BEGIN

class World: public QObject {
    Q_OBJECT
    Q_PROPERTY(Light *ambientLight READ ambientLight)
    Q_PROPERTY(QQmlListProperty<Object> objects READ qmlObjects)
    Q_CLASSINFO("DefaultProperty", "objects")

public:
    World(QObject *parent = nullptr);
    ~World(void) override;

public:
    virtual void tick(float elapsed);
    Light *ambientLight(void);
    const QList<Object *> &objects(void) const;
    QQmlListProperty<Object> qmlObjects(void);
    void appendObject(Object *object);
    qsizetype objectCount(void) const;
    Object *objectAt(qsizetype index);
    void clearObjects(void);
    void replaceObject(qsizetype index, Object *object);
    void removeLastObject(void);

signals:
    void objectAdded(Object *object);
    void objectRemoved(Object *object);

protected:
    virtual void handleObjectAdded(Object *object, bool emitSignal = true);
    virtual void handleObjectRemoved(Object *object, bool emitSignal = true);

private:
    static void qmlAppendObject(QQmlListProperty<Object> *list, Object *object);
    static qsizetype qmlObjectCount(QQmlListProperty<Object> *list);
    static Object *qmlObjectAt(QQmlListProperty<Object> *list, qsizetype index);
    static void qmlClearObjects(QQmlListProperty<Object> *list);
    static void qmlReplaceObject(QQmlListProperty<Object> *list, qsizetype index, Object *object);
    static void qmlRemoveLastObject(QQmlListProperty<Object> *list);

private:
    Light m_ambientLight;
    QList<Object *> m_objects;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_HPP
