//
//
#ifndef VOLCANO_PHYSICS_SPHERE_HPP
#define VOLCANO_PHYSICS_SPHERE_HPP

#include <Volcano/Physics/Common.hpp>
#include <Volcano/Physics/Shape.hpp>

VOLCANO_PHYSICS_BEGIN

class Sphere: public Shape {
public:
    Sphere(void);
    Sphere(qreal radius);
    ~Sphere(void) override;

public:
    qreal radius(void) const;
    void setRadius(qreal v);

private:
    qreal m_radius;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_SPHERE_HPP
