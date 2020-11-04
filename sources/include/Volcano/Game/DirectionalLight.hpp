//
//
#ifndef VOLCANO_GAME_DIRECTIONALLIGHT_HPP
#define VOLCANO_GAME_DIRECTIONALLIGHT_HPP

#include <QColor>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Light.hpp>

VOLCANO_GAME_BEGIN

class DirectionalLight: public Light
{
    Q_OBJECT

public:
    DirectionalLight(QObject *parent = nullptr);
    ~DirectionalLight(void) override;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_DIRECTIONALLIGHT_HPP
