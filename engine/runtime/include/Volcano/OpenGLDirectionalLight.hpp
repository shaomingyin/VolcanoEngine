//
//
#ifndef VOLCANO_OPENGLDIRECTIONALLIGHT_HPP
#define VOLCANO_OPENGLDIRECTIONALLIGHT_HPP

#include <Volcano/OpenGLCommon.hpp>
#include <Volcano/OpenGLLight.hpp>

VOLCANO_BEGIN

class OpenGLDirectionalLight: public OpenGLLight {
public:
    OpenGLDirectionalLight(void);
    ~OpenGLDirectionalLight(void) override;
};

VOLCANO_END

#endif // VOLCANO_OPENGLDIRECTIONALLIGHT_HPP
