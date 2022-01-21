//
//
#ifndef VOLCANO_PHYSICS_PLANE_HPP
#define VOLCANO_PHYSICS_PLANE_HPP

#include <Volcano/Physics/Common.hpp>
#include <Volcano/Physics/Shape.hpp>

VOLCANO_PHYSICS_BEGIN

class Plane: public Shape {
public:
    Plane(void);
    Plane(qreal x, qreal y, qreal z, qreal d);
    ~Plane(void) override;

public:
    qreal x(void) const;
    void setX(qreal v);
    qreal y(void) const;
    void setY(qreal v);
    qreal z(void) const;
    void setZ(qreal v);
    qreal d(void) const;
    void setD(qreal v);

private:
    qreal m_x;
    qreal m_y;
    qreal m_z;
    qreal m_d;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_PLANE_HPP
