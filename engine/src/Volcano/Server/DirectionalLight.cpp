//
//
#include <Volcano/Server/DirectionalLight.hpp>

VOLCANO_SERVER_BEGIN

DirectionalLight::DirectionalLight(void):
    m_direction(0.0f, -1.0f, 0.0f)
{
}

DirectionalLight::~DirectionalLight(void)
{
}

VOLCANO_SERVER_END
