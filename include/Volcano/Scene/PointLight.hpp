//
//
#ifndef VOLCANO_SCENE_POINTLIGHT_HPP
#define VOLCANO_SCENE_POINTLIGHT_HPP

#include <Volcano/Scene/Common.hpp>
#include <Volcano/Scene/Light.hpp>

VOLCANO_SCENE_BEGIN

class VOLCANO_API PointLight: public Light
{
    Q_OBJECT

public:
    PointLight(QObject *parent = nullptr);
    ~PointLight(void) override;
};

VOLCANO_SCENE_END

#endif // VOLCANO_SCENE_POINTLIGHT_HPP
