//
//
#include <Volcano/Graphics/Light.hpp>

VOLCANO_GRAPHICS_BEGIN

Light::Light(QObject *parent):
    Actor(parent)
{
}

Light::~Light(void)
{
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
