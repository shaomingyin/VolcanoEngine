//
//
#include <Volcano/Game/Plane.hpp>

VOLCANO_GAME_BEGIN

// Plane::Data

Plane::Data::Data(void):
    m_x(0.0f),
    m_y(0.0f),
    m_z(0.0f),
    m_d(0.0f)
{
}

Plane::Data::Data(const Data &that):
    m_x(that.m_x),
    m_y(that.m_y),
    m_z(that.m_z),
    m_d(that.m_d)
{
}

Plane::Data::Data(qreal x, qreal y, qreal z, qreal d):
    m_x(x),
    m_y(y),
    m_z(z),
    m_d(d)
{
}

qreal Plane::Data::x(void) const
{
    return m_x;
}

void Plane::Data::setX(qreal v)
{
    m_x = v;
}

qreal Plane::Data::y(void) const
{
    return m_y;
}

void Plane::Data::setY(qreal v)
{
    m_y = v;
}

qreal Plane::Data::z(void) const
{
    return m_z;
}

void Plane::Data::setZ(qreal v)
{
    m_z = v;
}

qreal Plane::Data::d(void) const
{
    return m_d;
}

void Plane::Data::setD(qreal v)
{
    m_d = v;
}

Plane::Data &Plane::Data::operator=(const Data &that)
{
    m_x = that.m_x;
    m_y = that.m_y;
    m_z = that.m_z;
    m_d = that.m_d;

    return (*this);
}

bool Plane::Data::operator==(const Data &that) const
{
    return (qFuzzyCompare(m_x, that.m_x) &&
            qFuzzyCompare(m_y, that.m_y) &&
            qFuzzyCompare(m_z, that.m_z) &&
            qFuzzyCompare(m_d, that.m_d));
}

bool Plane::Data::operator!=(const Data &that) const
{
    return !((*this) == that);
}

// Plane

Plane::Plane(QObject *parent):
    Shape(parent)
{
}

Plane::Plane(const Data &data, QObject *parent):
    Shape(parent),
    m_data(data)
{
}

Plane::Plane(qreal x, qreal y, qreal z, qreal d, QObject *parent):
    Shape(parent),
    m_data(x, y, z, d)
{
}

Plane::~Plane(void)
{
}

qreal Plane::x(void) const
{
    return m_data.x();
}

void Plane::setX(qreal v)
{
    m_data.setX(v);
    emit xChanged(v);
}

qreal Plane::y(void) const
{
    return m_data.y();
}

void Plane::setY(qreal v)
{
    m_data.setY(v);
    emit yChanged(v);
}

qreal Plane::z(void) const
{
    return m_data.z();
}

void Plane::setZ(qreal v)
{
    m_data.setZ(v);
    emit zChanged(v);
}

qreal Plane::d(void) const
{
    return m_data.d();
}

void Plane::setD(qreal v)
{
    m_data.setD(v);
    emit dChanged(v);
}

VOLCANO_GAME_END
