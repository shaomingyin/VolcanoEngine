//
//
#include <Volcano/Game/Sphere.hpp>

VOLCANO_GAME_BEGIN

// Sphere::Data

Sphere::Data::Data(void):
    m_radius(1.0f)
{
}

Sphere::Data::Data(const Data &that):
    m_radius(that.m_radius)
{
}

Sphere::Data::Data(qreal radius):
    m_radius(radius)
{
}

qreal Sphere::Data::radius(void) const
{
    return m_radius;
}

void Sphere::Data::setRadius(qreal v)
{
    m_radius = v;
}

Sphere::Data &Sphere::Data::operator=(const Data &that)
{
    m_radius = that.m_radius;

    return (*this);
}

bool Sphere::Data::operator==(const Data &that) const
{
    return qFuzzyCompare(m_radius, that.m_radius);
}

bool Sphere::Data::operator!=(const Data &that) const
{
    return !qFuzzyCompare(m_radius, that.m_radius);
}

// Sphere

Sphere::Sphere(QObject *parent):
    Shape(parent)
{
}

Sphere::~Sphere(void)
{
}

Sphere::Sphere(const Data &data, QObject *parent):
    Shape(parent),
    m_data(data)
{
}

Sphere::Sphere(qreal radius, QObject *parent):
    Shape(parent),
    m_data(radius)
{
}

const Sphere::Data &Sphere::data(void) const
{
    return m_data;
}

qreal Sphere::radius(void) const
{
    return m_data.radius();
}

void Sphere::setRadius(qreal v)
{
    m_data.setRadius(v);
    emit radiusChanged(v);
}

VOLCANO_GAME_END
