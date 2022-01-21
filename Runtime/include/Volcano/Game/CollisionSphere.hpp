//
//
#ifndef VOLCANO_GAME_COLLISIONSPHERE_HPP
#define VOLCANO_GAME_COLLISIONSPHERE_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/CollisionShape.hpp>

VOLCANO_GAME_BEGIN

class CollisionSphere: public CollisionShape {
    Q_OBJECT
    Q_PROPERTY(qreal radius READ radius WRITE setRadius NOTIFY radiusChanged)

public:
    CollisionSphere(QObject *parent = nullptr);
    CollisionSphere(qreal radius, QObject *parent = nullptr);
    ~CollisionSphere(void) override;

public:
    qreal radius(void) const;
    void setRadius(qreal v);

signals:
    void radiusChanged(qreal v);

private:
    qreal m_radius;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_COLLISIONSPHERE_HPP
