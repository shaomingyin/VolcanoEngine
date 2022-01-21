//
//
#include <Volcano/Physics/Box.hpp>

VOLCANO_PHYSICS_BEGIN

Box::Box(void):
    m_length(1.0f),
    m_width(1.0f),
    m_height(1.0f)
{
}

Box::Box(qreal length, qreal width, qreal height):
    m_length(length),
    m_width(width),
    m_height(height)
{
}

Box::~Box(void)
{
}

qreal Box::length(void) const
{
    return m_length;
}

void Box::setLength(qreal v)
{
    m_length = v;
}

qreal Box::width(void) const
{
    return m_width;
}

void Box::setWidth(qreal v)
{
    m_width = v;
}

qreal Box::height(void) const
{
    return m_height;
}

void Box::setHeight(qreal v)
{
    m_height = v;
}

VOLCANO_PHYSICS_END
