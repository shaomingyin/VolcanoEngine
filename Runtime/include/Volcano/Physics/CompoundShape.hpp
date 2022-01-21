//
//
#ifndef VOLCANO_PHYSICS_COMPOUNDSHAPE_HPP
#define VOLCANO_PHYSICS_COMPOUNDSHAPE_HPP

#include <Volcano/Physics/Common.hpp>
#include <Volcano/Physics/Shape.hpp>

VOLCANO_PHYSICS_BEGIN

class CompoundShape: public Shape {
public:
    CompoundShape(void);
    ~CompoundShape(void) override;
};

VOLCANO_PHYSICS_END

#endif // VOLCANO_PHYSICS_COMPOUNDSHAPE_HPP
