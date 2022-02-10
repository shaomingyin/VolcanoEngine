//
//
#ifndef VOLCANO_SYSTEM_QMLQUATERNIONUTILS_HPP
#define VOLCANO_SYSTEM_QMLQUATERNIONUTILS_HPP

#include <QObject>
#include <QQuaternion>

#include <Volcano/System/Common.hpp>

VOLCANO_SYSTEM_BEGIN

class QmlQuaternionUtils: public QObject {
    Q_OBJECT

public:
    QmlQuaternionUtils(QObject *parent = nullptr);
    ~QmlQuaternionUtils(void) override;

public:
    Q_INVOKABLE QQuaternion fromAxisAndAngle(qreal angle, const QVector3D &axis);
    Q_INVOKABLE QQuaternion fromAxisAndAngle(const QVector3D &axis, qreal angle);
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_QMLQUATERNIONUTILS_HPP
