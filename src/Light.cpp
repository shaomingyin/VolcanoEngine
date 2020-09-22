//
//
#include <Volcano/Light.hpp>

VOLCANO_BEGIN

Light::Light(QObject *parent):
    QObject(parent),
    m_color(QColor::fromRgbF(1.0f, 1.0f, 1.0f)),
    m_strength(0.5f)
{
}

Light::~Light(void)
{
}

VOLCANO_END
