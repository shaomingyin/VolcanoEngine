//
//
#ifndef VOLCANO_OPENGLSPOTLIGHT_HPP
#define VOLCANO_OPENGLSPOTLIGHT_HPP

#include <Volcano/Common.hpp>
#include <Volcano/OpenGLLight.hpp>

VOLCANO_BEGIN

class OpenGLSpotLight: public OpenGLLight {
public:
    OpenGLSpotLight(void);
    virtual ~OpenGLSpotLight(void);
};

VOLCANO_END

#endif // VOLCANO_OPENGLSPOTLIGHT_HPP
