//
//
#ifndef VOLCANO_SCENE_DIRECTIONALLIGHT_HPP
#define VOLCANO_SCENE_DIRECTIONALLIGHT_HPP

#include <QColor>

#include <Volcano/Scene/Common.hpp>
#include <Volcano/Scene/Light.hpp>

VOLCANO_SCENE_BEGIN

class VOLCANO_API DirectionalLight: public Light
{
    Q_OBJECT

public:
    DirectionalLight(QObject *parent = nullptr);
    ~DirectionalLight(void) override;
};

VOLCANO_SCENE_END

#endif // VOLCANO_SCENE_DIRECTIONALLIGHT_HPP
