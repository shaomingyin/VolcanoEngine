//
//
#include <Volcano/System/QmlQuaternionUtils.hpp>

VOLCANO_SYSTEM_BEGIN

QmlQuaternionUtils::QmlQuaternionUtils(QObject *parent):
    QObject(parent)
{
}

QmlQuaternionUtils::~QmlQuaternionUtils(void)
{
}

QQuaternion QmlQuaternionUtils::fromAxisAndAngle(qreal angle, const QVector3D &axis)
{
    return QQuaternion::fromAxisAndAngle(axis, angle);
}

QQuaternion QmlQuaternionUtils::fromAxisAndAngle(const QVector3D &axis, qreal angle)
{
    return QQuaternion::fromAxisAndAngle(axis, angle);
}

VOLCANO_SYSTEM_END
