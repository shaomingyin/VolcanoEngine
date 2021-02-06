//
//
#include <Volcano/Game/DirectionalLight.hpp>

VOLCANO_GAME_BEGIN

DirectionalLight::DirectionalLight(QObject *parent):
    Light(parent),
    m_direction(0.0f, -1.0f, 0.0f)
{
}

DirectionalLight::~DirectionalLight(void)
{
}

VOLCANO_GAME_END
