//
//
#include <Volcano/SpotLight.hpp>

VOLCANO_BEGIN

SpotLight::SpotLight(QObject *parent):
    DirectionalLight(parent)
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
    if (m_position != v) {
        m_position = v;
        emit positionChanged(v);
    }
}

const QVector3D &SpotLight::scale(void) const
{
    return m_scale;
}

void SpotLight::setScale(const QVector3D &v)
{
    if (m_scale != v) {
        m_scale = v;
        emit scaleChanged(v);
    }
}

VOLCANO_END
