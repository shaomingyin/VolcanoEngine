//
//
#include <Volcano/Rotation.hpp>

VOLCANO_BEGIN

Rotation::Rotation(QObject *parent):
    QObject(parent),
    m_angle(0.0f),
    m_axis(1.0f, 0.0f, 0.0f)
{
}

Rotation::~Rotation(void)
{
}

qreal Rotation::angle(void) const
{
    return m_angle;
}

void Rotation::setAngle(qreal v)
{
    m_angle = v;
    emit angleChanged(v);
}

const QVector3D &Rotation::axis(void) const
{
    return m_axis;
}

void Rotation::setAxis(const QVector3D &v)
{
    m_axis = v;
    emit axisChanged(v);
}

VOLCANO_END
