//
//
#include <Volcano/Box.hpp>

VOLCANO_BEGIN

Box::Box::Data::Data(void)
{
}

Box::Box::Data::Data(const Data &that):
    m_length(that.m_length),
    m_width(that.m_width),
    m_height(that.m_height)
{
}

Box::Box::Data::Data(float length, float width, float height):
    m_length(length),
    m_width(width),
    m_height(height)
{
}

Box::Box::Data::~Data(void)
{
}

float Box::Box::Data::length(void) const
{
    return m_length;
}

void Box::Box::Data::setLength(float v)
{
    m_length = v;
}

float Box::Box::Data::width(void) const
{
    return m_width;
}

void Box::Box::Data::setWidth(float v)
{
    m_width = v;
}

float Box::Box::Data::height(void) const
{
    return m_height;
}

void Box::Box::Data::setHeight(float v)
{
    m_height = v;
}

Box::Box::Data &Box::Box::Data::operator=(const Data &that)
{
    m_length = that.m_length;
    m_width = that.m_width;
    m_height = that.m_height;

    return (*this);
}

bool Box::Box::Data::operator==(const Data &that)
{
    return (qFuzzyCompare(m_length, that.m_length) &&
            qFuzzyCompare(m_width, that.m_width) &&
            qFuzzyCompare(m_height, that.m_height));
}

bool Box::Box::Data::operator!=(const Data &that)
{
    return !operator==(that);
}

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

void Box::setData(const Data &data)
{
    m_data = data;
    emit lengthChanged(m_data.length());
    emit widthChanged(m_data.width());
    emit heightChanged(m_data.height());
}

qreal Box::length(void) const
{
    return m_data.length();
}

void Box::setLength(qreal v)
{
    if (!qFuzzyCompare(qreal(m_data.length()), v)) {
        m_data.setLength(v);
        emit lengthChanged(v);
    }
}

qreal Box::width(void) const
{
    return m_data.width();
}

void Box::setWidth(qreal v)
{
    if (!qFuzzyCompare(qreal(m_data.width()), v)) {
        m_data.setWidth(v);
        emit widthChanged(v);
    }
}

qreal Box::height(void) const
{
    return m_data.height();
}

void Box::setHeight(qreal v)
{
    if (!qFuzzyCompare(qreal(m_data.height()), v)) {
        m_data.setHeight(v);
        emit heightChanged(v);
    }
}

VOLCANO_END
