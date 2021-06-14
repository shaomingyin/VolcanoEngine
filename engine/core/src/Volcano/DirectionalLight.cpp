//
//
#include <Volcano/DirectionalLight.hpp>

VOLCANO_BEGIN

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

VOLCANO_END
