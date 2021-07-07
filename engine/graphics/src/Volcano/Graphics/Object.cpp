//
//
#ifndef VOLCANO_GRAPHICS_OBJECT_HPP
#define VOLCANO_GRAPHICS_OBJECT_HPP

#include <Volcano/Game/Object.hpp>
#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class Object {
public:
    Object(void);
    virtual ~Object(void);

public:
    virtual void synchronize(void) = 0;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_OBJECT_HPP
