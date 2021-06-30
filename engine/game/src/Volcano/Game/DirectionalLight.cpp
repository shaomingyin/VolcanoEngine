//
//
#include <Volcano/Game/DirectionalLight.hpp>

VOLCANO_GAME_BEGIN

DirectionalLight::DirectionalLight(QObject *parent):
    Light(parent)
{
}

DirectionalLight::~DirectionalLight(void)
{
}

const QVector3D &DirectionalLight::direction(void) const
{
    return m_direction;
}

void DirectionalLight::setDirection(const QVector3D &v)
{
    if (m_direction != v) {
        m_direction = v;
        directionChanged(v);
    }
}

VOLCANO_GAME_END
