//
//
#include <Volcano/Game/Cylinder.hpp>

VOLCANO_GAME_BEGIN

// Cylinder::Data

Cylinder::Data::Data(void):
    m_radius(1.0f),
    m_height(1.0f)
{
}

Cylinder::Data::Data(const Data &that):
    m_radius(that.m_radius),
    m_height(that.m_height)
{
}

Cylinder::Data::Data(qreal radius, qreal height):
    m_radius(radius),
    m_height(height)
{
}

qreal Cylinder::Data::radius(void) const
{
    return m_radius;
}

void Cylinder::Data::setRadisu(qreal v)
{
    m_radius = v;
}

qreal Cylinder::Data::height(void) const
{
    return m_height;
}

void Cylinder::Data::setHeight(qreal v)
{
    m_height = v;
}

Cylinder::Data &Cylinder::Data::operator=(const Data &that)
{
    m_radius = that.m_radius;
    m_height = that.m_height;

    return (*this);
}

bool Cylinder::Data::operator==(const Data &that) const
{
    return (qFuzzyCompare(m_radius, that.m_radius) &&
            qFuzzyCompare(m_height, that.m_height));
}

bool Cylinder::Data::operator!=(const Data &that) const
{
    return !((*this) == that);
}

// Cylinder

Cylinder::Cylinder(QObject *parent):
    Shape(parent)
{
}

Cylinder::Cylinder(const Data &data, QObject *parent):
    Shape(parent),
    m_data(data)
{
}

Cylinder::Cylinder(qreal radius, qreal height, QObject *parent):
    Shape(parent),
    m_data(radius, height)
{
}

Cylinder::~Cylinder(void)
{
}

const Cylinder::Data &Cylinder::data(void) const
{
    return m_data;
}

qreal Cylinder::radius(void) const
{
    return m_data.radius();
}

void Cylinder::setRadius(qreal v)
{
    m_data.setRadisu(v);
    emit radiusChanged(v);
}

qreal Cylinder::height(void) const
{
    return m_data.height();
}

void Cylinder::setHeight(qreal v)
{
    m_data.setHeight(v);
    emit heightChanged(v);
}

VOLCANO_GAME_END
