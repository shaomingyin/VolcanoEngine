//
//
#ifndef VOLCANO_GAME_GRAPHICS_AMBIENTLIGHT_HPP
#define VOLCANO_GAME_GRAPHICS_AMBIENTLIGHT_HPP

#include <Volcano/Game/Graphics/Common.hpp>
#include <Volcano/Game/Graphics/Light.hpp>

VOLCANO_GAME_GRAPHICS_BEGIN

class AmbientLight: public Light {
    Q_OBJECT

public:
    AmbientLight(QObject *parent = nullptr);
    ~AmbientLight(void);
};

VOLCANO_GAME_GRAPHICS_END

#endif // VOLCANO_GAME_GRAPHICS_AMBIENTLIGHT_HPP
