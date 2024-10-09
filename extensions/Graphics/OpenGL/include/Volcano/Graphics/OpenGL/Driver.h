//
//
#ifndef VOLCANO_GRAPHICS_OPENGL_DRIVER_H
#define VOLCANO_GRAPHICS_OPENGL_DRIVER_H

#include <Volcano/Graphics/Driver.h>
#include <Volcano/Graphics/OpenGL/Common.h>

VOLCANO_GRAPHICS_OPENGL_BEGIN

class Driver: public Graphics::Driver {
public:
    Driver() = default;
    virtual ~Driver() = default;

public:
    bool init() override;
};

VOLCANO_GRAPHICS_OPENGL_END

#endif // VOLCANO_GRAPHICS_OPENGL_DRIVER_H
