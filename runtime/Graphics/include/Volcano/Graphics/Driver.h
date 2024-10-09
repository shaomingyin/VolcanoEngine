//
//
#ifndef VOLCANO_GRAPHICS_DRIVER_H
#define VOLCANO_GRAPHICS_DRIVER_H

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Driver {
public:
    Driver() = default;
    virtual ~Driver() = default;

public:
    virtual bool init() = 0;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_DRIVER_H
