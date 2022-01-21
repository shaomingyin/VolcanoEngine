//
//
#include <Volcano/Graphics/Light.hpp>

#ifdef VOLCANO_GRAPHICS

VOLCANO_GRAPHICS_BEGIN

Light::Light(void)
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
    m_color = v;
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS
