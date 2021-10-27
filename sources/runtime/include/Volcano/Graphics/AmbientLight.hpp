//
//
#ifndef VOLCANO_GRAPHICS_AMBIENTLIGHT_HPP
#define VOLCANO_GRAPHICS_AMBIENTLIGHT_HPP

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Light.hpp>

VOLCANO_GRAPHICS_BEGIN

class AmbientLight: public Light {
    Q_OBJECT

public:
    AmbientLight(QObject *parent = nullptr);
    ~AmbientLight(void) override;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_AMBIENTLIGHT_HPP
