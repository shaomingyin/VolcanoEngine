//
//
#ifndef VOLCANO_SYSTEM_QUATERNIONUTILS_HPP
#define VOLCANO_SYSTEM_QUATERNIONUTILS_HPP

#include <QObject>
#include <QQuaternion>

#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

class QuaternionUtils: public QObject {
    Q_OBJECT

public:
    QuaternionUtils(QObject *parent = nullptr);
    ~QuaternionUtils(void) override;

public:
    Q_INVOKABLE QQuaternion fromAxisAndAngle(qreal angle, const QVector3D &axis);
    Q_INVOKABLE QQuaternion fromAxisAndAngle(const QVector3D &axis, qreal angle);
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_QUATERNIONUTILS_HPP
