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
#include <Volcano/Game/WorldBase.hpp>

VOLCANO_GAME_BEGIN

class World: public WorldBase {
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Object> objects READ qmlObjects)
    Q_CLASSINFO("DefaultProperty", "objects")

public:
    World(QObject *parent = nullptr);
    ~World(void) override;

public:
    QQmlListProperty<Object> qmlObjects(void);

protected:
    void handleObjectAdded(Object *object, bool emitSignal) override;
    void handleObjectRemoved(Object *object, bool emitSignal) override;

private:
    static void qmlAppendObject(QQmlListProperty<Object> *list, Object *object);
    static qsizetype qmlObjectCount(QQmlListProperty<Object> *list);
    static Object *qmlObjectAt(QQmlListProperty<Object> *list, qsizetype index);
    static void qmlClearObjects(QQmlListProperty<Object> *list);
    static void qmlReplaceObject(QQmlListProperty<Object> *list, qsizetype index, Object *object);
    static void qmlRemoveLastObject(QQmlListProperty<Object> *list);
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLD_HPP
