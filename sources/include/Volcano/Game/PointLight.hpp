//
//
#ifndef VOLCANO_GAME_POINTLIGHT_HPP
#define VOLCANO_GAME_POINTLIGHT_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Light.hpp>

VOLCANO_GAME_BEGIN

class PointLight: public Light
{
    Q_OBJECT

public:
    PointLight(QObject *parent = nullptr);
    ~PointLight(void) override;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_POINTLIGHT_HPP
