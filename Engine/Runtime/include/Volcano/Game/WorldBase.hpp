//
//
#ifndef VOLCANO_GAME_WORLDBASE_HPP
#define VOLCANO_GAME_WORLDBASE_HPP

#include <QList>
#include <QString>
#include <QObject>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Object.hpp>

VOLCANO_GAME_BEGIN

class WorldBase: public QObject {
    Q_OBJECT

public:
    WorldBase(QObject *parent = nullptr);
    ~WorldBase(void) override;

public:
    virtual void tick(Duration elapsed);
    const QList<Object *> &objects(void) const;
    void appendObject(Object *object);
    qsizetype objectCount(void) const;
    Object *objectAt(qsizetype index);
    const Object *objectAt(qsizetype index) const;
    void clearObjects(void);
    void replaceObject(qsizetype index, Object *object);
    void removeLastObject(void);

signals:
    void objectAdded(Object *object);
    void objectRemoved(Object *object);

protected:
    virtual void handleObjectAdded(Object *object, bool emitSignal = true);
    virtual void handleObjectRemoved(Object *object, bool emitSignal = true);

    QList<Object *> m_objects;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_WORLDBASE_HPP
