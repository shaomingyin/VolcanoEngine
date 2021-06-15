//
//
#ifndef VOLCANO_OPENGL_POINTLIGHT_HPP
#define VOLCANO_OPENGL_POINTLIGHT_HPP

#include <Volcano/OpenGL/Common.hpp>
#include <Volcano/OpenGL/Light.hpp>

VOLCANO_OPENGL_BEGIN

class PointLight: public Light {
public:
    PointLight(void);
    ~PointLight(void) override;
};

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_POINTLIGHT_HPP
