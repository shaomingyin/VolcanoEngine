//
//
#ifndef VOLCANO_UI_GRPHICS_SPOTLIGHT_HPP
#define VOLCANO_UI_GRPHICS_SPOTLIGHT_HPP

#include <QObject>

#include <Volcano/UI/Graphics/Common.hpp>
#include <Volcano/UI/Graphics/Light.hpp>

VOLCANO_UI_GRAPHICS_BEGIN

class VOLCANO_UI_API SpotLight: public Light {
    Q_OBJECT

public:
    SpotLight(QObject *parent = nullptr);
    ~SpotLight(void) override;
};

VOLCANO_UI_GRAPHICS_END

#endif // VOLCANO_UI_GRPHICS_MESH_HPP
