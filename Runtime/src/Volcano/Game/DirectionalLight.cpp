//
//
#include <Volcano/Game/DirectionalLight.hpp>

VOLCANO_GAME_BEGIN

DirectionalLight::DirectionalLight(QObject *parent)
    : Light(parent)
    , m_direction(0.0f, -1.0f, 0.0f)
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
    if (!qFuzzyCompare(m_direction, v)) {
        m_direction = v;
        emit directionChanged(v);
    }
}

VOLCANO_GAME_END
