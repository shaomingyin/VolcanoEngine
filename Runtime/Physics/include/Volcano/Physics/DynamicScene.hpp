//
//
#ifndef VOLCANO_PHYSICS_DYNAMICSCENE_HPP
#define VOLCANO_PHYSICS_DYNAMICSCENE_HPP

#include <QVector3D>
#include <QScopedPointer>

#include <btBulletDynamicsCommon.h>

#include <Volcano/Scene.hpp>
#include <Volcano/Physics/Common.hpp>

VOLCANO_PHYSICS_BEGIN

class DynamicScene: public Scene {
    Q_OBJECT
    Q_PROPERTY(QVector3D gravity READ gravity WRITE setGravity NOTIFY gravityChanged)

public:
    DynamicScene(QObject* parent = nullptr);
    DynamicScene(Context& context, QObject* parent = nullptr);

public:
    const QVector3D& gravity(void) const;
    void setGravity(const QVector3D& v);

signals:
    void gravityChanged(const QVector3D& v);

private:
    QVector3D gravity_;
    QScopedPointer<btDiscreteDynamicsWorld> btDynamicsWorld_;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_DYNAMICSCENE_HPP
