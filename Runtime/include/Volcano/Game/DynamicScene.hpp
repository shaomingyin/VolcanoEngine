//
//
#ifndef VOLCANO_GAME_DYNAMICSCENE_HPP
#define VOLCANO_GAME_DYNAMICSCENE_HPP

#include <QVector3D>
#include <QScopedPointer>

#include <bullet/BulletDynamics/btBulletDynamicsCommon.h>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Scene.hpp>

VOLCANO_GAME_BEGIN

class DynamicScene: public Scene {
    Q_OBJECT
    Q_PROPERTY(QVector3D gravity READ gravity WRITE setGravity NOTIFY gravityChanged)

public:
    DynamicScene(QObject* parent = nullptr);
    ~DynamicScene(void) override;

public:
    const QVector3D& gravity(void) const;
    void setGravity(const QVector3D& v);

signals:
    void gravityChanged(const QVector3D& v);

private:
    QVector3D gravity_;
    QScopedPointer<btDiscreteDynamicsWorld> btDynamicsWorld_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_DYNAMICSCENE_HPP
