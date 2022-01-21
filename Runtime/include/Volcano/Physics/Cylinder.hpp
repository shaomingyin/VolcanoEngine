//
//
#ifndef VOLCANO_PHYSICS_CYLINDER_HPP
#define VOLCANO_PHYSICS_CYLINDER_HPP

#include <Volcano/Physics/Common.hpp>
#include <Volcano/Physics/Sphere.hpp>

VOLCANO_PHYSICS_BEGIN

class Cylinder: public Sphere {
public:
    Cylinder(void);
    Cylinder(qreal radius, qreal height);
    ~Cylinder(void) override;

public:
    qreal height(void) const;
    void setHeight(qreal v);

private:
    qreal m_height;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_CYLINDER_HPP
