//
//
#include <Volcano/Game/SpotLight.hpp>

VOLCANO_GAME_BEGIN

SpotLight::SpotLight(QObject *parent)
    : DirectionalLight(parent)
{
}

SpotLight::~SpotLight(void)
{
}

const QVector3D &SpotLight::position(void) const
{
    return m_position;
}

void SpotLight::setPosition(const QVector3D &v)
{
    if (!qFuzzyCompare(m_position, v)) {
        m_position = v;
        emit positionChanged(v);
    }
}

VOLCANO_GAME_END
