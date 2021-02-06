//
//
#ifndef VOLCANO_GRPHICS_POINTLIGHT_HPP
#define VOLCANO_GRPHICS_POINTLIGHT_HPP

#include <QObject>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Light.hpp>

VOLCANO_GRAPHICS_BEGIN

class VOLCANO_API PointLight: public Light
{
    Q_OBJECT

public:
    PointLight(QObject *parent = nullptr);
    ~PointLight(void) override;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRPHICS_POINTLIGHT_HPP
