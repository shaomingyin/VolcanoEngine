//
//
#include <Volcano/Game/Light.hpp>

VOLCANO_GAME_BEGIN

Light::Light(QObject *parent)
    : Actor(parent)
    , m_color(QColor::fromRgb(255, 255, 255))
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

VOLCANO_GAME_END
