//
//
#ifndef VOLCANO_PHYSICS_WORLD_HPP
#define VOLCANO_PHYSICS_WORLD_HPP

#include <QVector3D>
#include <QObject>

#include <Volcano/Physics/Common.hpp>

VOLCANO_PHYSICS_BEGIN

class VOLCANO_PHYSICS_API World: public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector3D gravity READ gravity WRITE setGravity NOTIFY gravityChanged)

public:
    World(QObject* parent = nullptr);

public:
    const QVector3D& gravity(void) const;
    void setGravity(const QVector3D& v);

signals:
    void gravityChanged(const QVector3D& v);

private:
    QVector3D gravity_;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_WORLD_HPP
