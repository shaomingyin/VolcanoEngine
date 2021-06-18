//
//
#ifndef VOLCANO_DYNAMICWORLD_HPP
#define VOLCANO_DYNAMICWORLD_HPP

#include <QList>
#include <QObject>
#include <QVector3D>
#include <QQmlListProperty>

#include <bullet/btBulletDynamicsCommon.h>

#include <Volcano/Common.hpp>
#include <Volcano/World.hpp>

VOLCANO_BEGIN

class DynamicWorld: public World {
    Q_OBJECT
    Q_PROPERTY(QVector3D gravity READ gravity WRITE setGravity NOTIFY gravityChanged)

public:
    DynamicWorld(QObject *parent = nullptr);
    ~DynamicWorld(void) override;

public:
    void update(float elapsed) override;
    const QVector3D &gravity(void) const;
    void setGravity(const QVector3D &v);

signals:
    void gravityChanged(const QVector3D &v);

private:
    btDynamicsWorld *m_btWorld;
    QVector3D m_gravity;
};

VOLCANO_END

#endif // VOLCANO_DYNAMICWORLD_HPP
