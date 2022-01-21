//
//
#ifndef VOLCANO_GAME_COLLISIONPLANE_HPP
#define VOLCANO_GAME_COLLISIONPLANE_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/CollisionShape.hpp>

VOLCANO_GAME_BEGIN

class CollisionPlane: public CollisionShape {
    Q_OBJECT
    Q_PROPERTY(qreal x READ x WRITE setX NOTIFY xChanged)
    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged)
    Q_PROPERTY(qreal z READ z WRITE setZ NOTIFY zChanged)
    Q_PROPERTY(qreal d READ d WRITE setD NOTIFY dChanged)

public:
    CollisionPlane(QObject *parent = nullptr);
    CollisionPlane(qreal x, qreal y, qreal z, qreal d, QObject *parent = nullptr);
    ~CollisionPlane(void) override;

public:
    qreal x(void) const;
    void setX(qreal v);
    qreal y(void) const;
    void setY(qreal v);
    qreal z(void) const;
    void setZ(qreal v);
    qreal d(void) const;
    void setD(qreal v);

signals:
    void xChanged(qreal v);
    void yChanged(qreal v);
    void zChanged(qreal v);
    void dChanged(qreal v);

private:
    qreal m_x;
    qreal m_y;
    qreal m_z;
    qreal m_d;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_COLLISIONPLANE_HPP
