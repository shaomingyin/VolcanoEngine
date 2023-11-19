//
//
#ifndef VOLCANO_GAME_CAMERA_HPP
#define VOLCANO_GAME_CAMERA_HPP

#include <QVector3D>
#include <QMatrix4x4>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Object.hpp>

VOLCANO_GAME_BEGIN

class Camera: public Object {
    Q_OBJECT

public:
    Camera(QObject *parent = nullptr);

public:
    const QVector3D &position() const;
    void setPosition(const QVector3D &v);
    const QVector3D &direction() const;
    void setDirection(const QVector3D &v);
    const QVector3D &up() const;
    void setUp(const QVector3D &v);
    void lookTo(const QVector3D &position, const QVector3D &direction, const QVector3D &up);
    void toViewMatrix(QMatrix4x4 &out);

signals:
    void positionChanged(const QVector3D &v);
    void directionChanged(const QVector3D &v);
    void upChanged(const QVector3D &v);

private:
    QVector3D position_;
    QVector3D direction_;
    QVector3D up_;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CAMERA_HPP
