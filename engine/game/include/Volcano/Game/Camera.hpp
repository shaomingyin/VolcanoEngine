//
//
#ifndef VOLCANO_GAME_CAMERA_HPP
#define VOLCANO_GAME_CAMERA_HPP

#include <QObject>
#include <QVector3D>
#include <QRectF>

#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class VOLCANO_GAME_API Camera: public QObject {
    Q_OBJECT

public:
    Camera(QObject *parent = nullptr);
    ~Camera(void) override;

public:
    const QVector3D &position(void) const;
    void setPosition(const QVector3D &r);
    const QVector3D &direction(void) const;
    void setDirection(const QVector3D &r);
    const QVector3D &up(void) const;
    void setUp(const QVector3D &r);
    const QRectF &rect(void) const;
    void setRect(const QRectF &r);
    virtual void update(float elapsed) {};

private:
    QVector3D m_position;
    QVector3D m_direction;
    QVector3D m_up;
    QRectF m_rect;
};

VOLCANO_INLINE const QVector3D &Camera::position(void) const
{
    return m_position;
}

VOLCANO_INLINE void Camera::setPosition(const QVector3D &r)
{
    m_position = r;
}

VOLCANO_INLINE const QVector3D &Camera::direction(void) const
{
    return m_direction;
}

VOLCANO_INLINE void Camera::setDirection(const QVector3D &r)
{
    m_direction = r;
}

VOLCANO_INLINE const QVector3D &Camera::up(void) const
{
    return m_up;
}

VOLCANO_INLINE void Camera::setUp(const QVector3D &r)
{
    m_up = r;
}

VOLCANO_INLINE const QRectF &Camera::rect(void) const
{
    return m_rect;
}

VOLCANO_INLINE void Camera::setRect(const QRectF &r)
{
    m_rect = r;
}

VOLCANO_GAME_END

#endif // VOLCANO_GAME_CAMERA_HPP
