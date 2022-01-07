//
//
#include <Volcano/Box.hpp>

VOLCANO_BEGIN

Box::Box(QObject *parent):
    Shape(parent),
    m_length(1.0f),
    m_width(1.0f),
    m_height(1.0f)
{
}

Box::Box(qreal length, qreal width, qreal height, QObject *parent):
    Shape(parent),
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
    emit lengthChanged(v);
}

qreal Box::width(void) const
{
    return m_width;
}

void Box::setWidth(qreal v)
{
    m_width = v;
    emit widthChanged(v);
}

qreal Box::height(void) const
{
    return m_height;
}

void Box::setHeight(qreal v)
{
    m_height = v;
    emit heightChanged(v);
}

VOLCANO_END
