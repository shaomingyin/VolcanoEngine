//
//
#ifndef VOLCANO_OPENGL_SPOTLIGHT_HPP
#define VOLCANO_OPENGL_SPOTLIGHT_HPP

#include <Volcano/OpenGL/Common.hpp>
#include <Volcano/OpenGL/Light.hpp>

VOLCANO_OPENGL_BEGIN

class SpotLight: public Light {
public:
    SpotLight(void);
    virtual ~SpotLight(void);
};

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_MESH_HPP
