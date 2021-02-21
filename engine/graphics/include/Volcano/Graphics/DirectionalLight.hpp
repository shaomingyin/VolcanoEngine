//
//
#ifndef VOLCANO_GRAPHICS_DIRECTIONALLIGHT_HPP
#define VOLCANO_GRAPHICS_DIRECTIONALLIGHT_HPP

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Light.hpp>

VOLCANO_GRAPHICS_BEGIN

class VOLCANO_GRAPHICS_API DirectionalLight: public Light {
    Q_OBJECT

public:
    DirectionalLight(QObject *parent = nullptr);
    ~DirectionalLight(void) override;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_DIRECTIONALLIGHT_HPP
