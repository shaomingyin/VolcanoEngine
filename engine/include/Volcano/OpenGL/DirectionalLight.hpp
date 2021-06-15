//
//
#ifndef VOLCANO_OPENGL_DIRECTIONALLIGHT_HPP
#define VOLCANO_OPENGL_DIRECTIONALLIGHT_HPP

#include <Volcano/OpenGL/Common.hpp>
#include <Volcano/OpenGL/Light.hpp>

VOLCANO_OPENGL_BEGIN

class DirectionalLight: public Light {
public:
    DirectionalLight(void);
    ~DirectionalLight(void) override;
};

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_DIRECTIONALLIGHT_HPP
