//
//
#ifndef VOLCANO_GAME_SPOTLIGHT_HPP
#define VOLCANO_GAME_SPOTLIGHT_HPP

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Light.hpp>

VOLCANO_GAME_BEGIN

class SpotLight: public Light
{
    Q_OBJECT

public:
    SpotLight(QObject *parent = nullptr);
    ~SpotLight(void) override;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_SPOTLIGHT_HPP
