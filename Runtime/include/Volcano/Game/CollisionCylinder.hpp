//
//
#ifndef VOLCANO_GAME_COLLISIONCYLINDER_HPP
#define VOLCANO_GAME_COLLISIONCYLINDER_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/CollisionSphere.hpp>

VOLCANO_GAME_BEGIN

class CollisionCylinder: public CollisionSphere {
    Q_OBJECT
    Q_PROPERTY(qreal height READ height WRITE setHeight NOTIFY heightChanged)

public:
    CollisionCylinder(QObject *parent = nullptr);
    CollisionCylinder(qreal radius, qreal height, QObject *parent = nullptr);
    ~CollisionCylinder(void) override;

public:
    qreal height(void) const;
    void setHeight(qreal v);

signals:
    void heightChanged(qreal v);

private:
    qreal m_height;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_COLLISIONCYLINDER_HPP
