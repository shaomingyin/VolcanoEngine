//
//
#include <Volcano/Game/Box.hpp>

VOLCANO_GAME_BEGIN

// Box::Data

Box::Data::Data(void):
    m_length(1.0f),
    m_width(1.0f),
    m_height(1.0f)
{
}

Box::Data::Data(const Data &that)
{
}

Box::Data::Data(qreal length, qreal width, qreal height):
    m_length(length),
    m_width(width),
    m_height(height)
{
}

qreal Box::Data::length(void) const
{
    return m_length;
}

void Box::Data::setLength(qreal v)
{
    m_length = v;
}

qreal Box::Data::width(void) const
{
    return m_width;
}

void Box::Data::setWidth(qreal v)
{
    m_width = v;
}

qreal Box::Data::height(void) const
{
    return m_height;
}

void Box::Data::setHeight(qreal v)
{
    m_height = v;
}

Box::Data &Box::Data::operator=(const Data &that)
{
    m_length = that.m_length;
    m_width = that.m_width;
    m_height = that.m_height;

    return (*this);
}

bool Box::Data::operator==(const Data &that) const
{
    return (qFuzzyCompare(m_length, that.m_length) &&
            qFuzzyCompare(m_width, that.m_width) &&
            qFuzzyCompare(m_height, that.m_height));
}

bool Box::Data::operator!=(const Data &that) const
{
    return !((*this) == that);
}

// Box

Box::Box(QObject *parent):
    Shape(parent)
{
}

Box::Box(const Data &data, QObject *parent):
    Shape(parent),
    m_data(data)
{
}

Box::Box(qreal length, qreal width, qreal height, QObject *parent):
    Shape(parent),
    m_data(length, width, height)
{
}

Box::~Box(void)
{
}

const Box::Data &Box::data(void) const
{
    return m_data;
}

qreal Box::length(void) const
{
    return m_data.length();
}

void Box::setLength(qreal v)
{
    m_data.setLength(v);
    emit lengthChanged(v);
}

qreal Box::width(void) const
{
    return m_data.width();
}

void Box::setWidth(qreal v)
{
    m_data.setWidth(v);
    emit widthChanged(v);
}

qreal Box::height(void) const
{
    return m_data.height();
}

void Box::setHeight(qreal v)
{
    m_data.setHeight(v);
    emit heightChanged(v);
}

VOLCANO_GAME_END
