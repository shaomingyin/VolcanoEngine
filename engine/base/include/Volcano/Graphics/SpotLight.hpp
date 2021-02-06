//
//
#ifndef VOLCANO_GRPHICS_SPOTLIGHT_HPP
#define VOLCANO_GRPHICS_SPOTLIGHT_HPP

#include <QObject>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Light.hpp>

VOLCANO_GRAPHICS_BEGIN

class VOLCANO_API SpotLight: public Light
{
    Q_OBJECT

public:
    SpotLight(QObject *parent = nullptr);
    ~SpotLight(void) override;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRPHICS_MESH_HPP
