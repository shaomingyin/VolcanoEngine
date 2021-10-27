//
//
#include <Volcano/Graphics/Light.hpp>

VOLCANO_GRAPHICS_BEGIN

Light::Light(QObject *parent):
    Object(parent)
{
}

Light::~Light(void)
{
}

float Light::strength(void) const
{
    return m_strength;
}

void Light::setStrength(float v)
{
    if (!qFuzzyCompare(m_strength, v)) {
        m_strength = v;
        emit strengthChanged(v);
    }
}

const QColor &Light::color(void) const
{
    return m_color;
}

void Light::setColor(const QColor &v)
{
    if (m_color != v) {
        m_color = v;
        emit colorChanged(v);
    }
}

VOLCANO_GRAPHICS_END
