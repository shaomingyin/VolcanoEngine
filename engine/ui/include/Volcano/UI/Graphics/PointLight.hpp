//
//
#ifndef VOLCANO_UI_GRPHICS_POINTLIGHT_HPP
#define VOLCANO_UI_GRPHICS_POINTLIGHT_HPP

#include <QObject>

#include <Volcano/UI/Graphics/Common.hpp>
#include <Volcano/UI/Graphics/Light.hpp>

VOLCANO_UI_GRAPHICS_BEGIN

class VOLCANO_UI_API PointLight: public Light {
    Q_OBJECT

public:
    PointLight(QObject *parent = nullptr);
    ~PointLight(void) override;
};

VOLCANO_UI_GRAPHICS_END

#endif // VOLCANO_UI_GRPHICS_POINTLIGHT_HPP
