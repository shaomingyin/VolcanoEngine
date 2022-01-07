//
//
#ifndef VOLCANO_ROTATION_HPP
#define VOLCANO_ROTATION_HPP

#include <QObject>
#include <QVector3D>
#include <QQuaternion>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class Rotation: public QObject {
    Q_OBJECT
    Q_PROPERTY(qreal angle READ angle WRITE setAngle NOTIFY angleChanged)
    Q_PROPERTY(QVector3D axis READ axis WRITE setAxis NOTIFY axisChanged)

public:
    Rotation(QObject *parent = nullptr);
    ~Rotation(void) override;

public:
    qreal angle(void) const;
    void setAngle(qreal v);
    const QVector3D &axis(void) const;
    void setAxis(const QVector3D &v);

signals:
    void angleChanged(qreal v);
    void axisChanged(const QVector3D &v);

private:
    qreal m_angle;
    QVector3D m_axis;
};

VOLCANO_END

#endif // VOLCANO_ROTATION_HPP
