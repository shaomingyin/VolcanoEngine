//
//
#include <Volcano/Game/SpotLight.hpp>

VOLCANO_GAME_BEGIN

SpotLight::SpotLight(QObject *parent):
    DirectionalLight(parent),
    m_distance(1.0f)
{
}

SpotLight::SpotLight(Context *context, QObject *parent):
    DirectionalLight(context, parent),
    m_distance(1.0f)
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

qreal SpotLight::distance(void) const
{
    return m_distance;
}

void SpotLight::setDistance(qreal v)
{
    if (!qFuzzyCompare(m_distance, v)) {
        m_distance = v;
        emit distanceChanged(v);
    }
}

VOLCANO_GAME_END
