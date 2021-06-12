//
//
#ifndef VOLCANO_GAME_OBJECT_HPP
#define VOLCANO_GAME_OBJECT_HPP

#include <QObject>
#include <QVector3D>
#include <QQuaternion>

#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class Object: public QObject {
    Q_OBJECT
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QQuaternion rotation READ rotation WRITE setRotation NOTIFY rotationChanged)

public:
    Object(QObject *parent = nullptr);
    ~Object(void) override;

public:
    bool isEnabled(void) const;
    void setEnabled(bool v);
    const QVector3D &position(void) const;
    void setPosition(const QVector3D &v);
    const QVector3D &scale(void) const;
    void setScale(const QVector3D &v);
    const QQuaternion &rotation(void) const;
    void setRotation(const QQuaternion &v);

signals:
    void enabledChanged(bool v);
    void positionChanged(const QVector3D &v);
    void scaleChanged(const QVector3D &v);
    void rotationChanged(const QQuaternion &v);

private:
    bool m_isEnabled;
    QVector3D m_position;
    QVector3D m_scale;
    QQuaternion m_rotation;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_OBJECT_HPP
