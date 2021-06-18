//
//
#ifndef VOLCANO_OPENGLPOINTLIGHT_HPP
#define VOLCANO_OPENGLPOINTLIGHT_HPP

#include <Volcano/Common.hpp>
#include <Volcano/OpenGLLight.hpp>

VOLCANO_BEGIN

class OpenGLPointLight: public OpenGLLight {
public:
    OpenGLPointLight(void);
    ~OpenGLPointLight(void) override;
};

VOLCANO_END

#endif // VOLCANO_OPENGLPOINTLIGHT_HPP
