//
//
#include <Volcano/Game/PointLight.hpp>

VOLCANO_GAME_BEGIN

PointLight::PointLight(QObject *parent)
    : Light(parent)
{
}

PointLight::~PointLight(void)
{
}

const QVector3D &PointLight::position(void) const
{
    return m_position;
}

void PointLight::setPosition(const QVector3D &v)
{
    if (!qFuzzyCompare(m_position, v)) {
        m_position = v;
        emit positionChanged(v);
    }
}

VOLCANO_GAME_END
