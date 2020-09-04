//
//
#ifndef VOLCANO_SCENE_SPOTLIGHT_HPP
#define VOLCANO_SCENE_SPOTLIGHT_HPP

#include <Volcano/Scene/Common.hpp>
#include <Volcano/Scene/Light.hpp>

VOLCANO_SCENE_BEGIN

class VOLCANO_API SpotLight: public Light
{
    Q_OBJECT

public:
    SpotLight(QObject *parent = nullptr);
    ~SpotLight(void) override;
};

VOLCANO_SCENE_END

#endif // VOLCANO_SCENE_SPOTLIGHT_HPP
