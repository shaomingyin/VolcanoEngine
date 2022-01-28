//
//
#include <Volcano/System/QuaternionUtils.hpp>

VOLCANO_SYSTEM_BEGIN

QuaternionUtils::QuaternionUtils(QObject *parent):
    QObject(parent)
{
}

QuaternionUtils::~QuaternionUtils(void)
{
}

QQuaternion QuaternionUtils::fromAxisAndAngle(qreal angle, const QVector3D &axis)
{
    return QQuaternion::fromAxisAndAngle(axis, angle);
}

QQuaternion QuaternionUtils::fromAxisAndAngle(const QVector3D &axis, qreal angle)
{
    return QQuaternion::fromAxisAndAngle(axis, angle);
}

VOLCANO_SYSTEM_END
