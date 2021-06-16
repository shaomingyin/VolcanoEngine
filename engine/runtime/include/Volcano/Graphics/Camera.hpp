//
//
#ifndef VOLCANO_GRAPHICS_CAMERA_HPP
#define VOLCANO_GRAPHICS_CAMERA_HPP

#include <QRectF>
#include <QVector3D>
#include <QMatrix4x4>
#include <QQuickFramebufferObject>

#include <Volcano/World.hpp>
#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/CameraRenderer.hpp>

VOLCANO_GRAPHICS_BEGIN

class Camera: public QQuickFramebufferObject {
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(QVector3D up READ up WRITE setUp NOTIFY upChanged)
    Q_PROPERTY(QRectF rect READ rect WRITE setRect NOTIFY rectChanged)
    Q_PROPERTY(float nearPlane READ nearPlane WRITE setNearPlane NOTIFY nearPlaneChanged)
    Q_PROPERTY(float farPlane READ farPlane WRITE setFarPlane NOTIFY farPlaneChanged)
    Q_PROPERTY(int fps READ fps NOTIFY fpsChanged)
    Q_PROPERTY(int fpsMax READ fpsMax WRITE setFpsMax NOTIFY fpsMaxChanged)
    Q_PROPERTY(World *world READ world WRITE setWorld NOTIFY worldChanged)

public:
    Camera(QQuickItem *parent = nullptr);
    ~Camera(void) override;

public:
    Renderer *createRenderer(void) const override;
    const QVector3D &position(void) const;
    void setPosition(const QVector3D &v);
    const QVector3D &direction(void) const;
    void setDirection(const QVector3D &v);
    const QVector3D &up(void) const;
    void setUp(const QVector3D &v);
    const QRectF &rect(void) const;
    void setRect(const QRectF &v);
    float nearPlane(void) const;
    void setNearPlane(float v);
    float farPlane(void) const;
    void setFarPlane(float v);
    int fps(void) const;
    int fpsMax(void) const;
    void setFpsMax(int v);
    World *world(void);
    void setWorld(World *world);

signals:
    void positionChanged(const QVector3D &v);
    void directionChanged(const QVector3D &v);
    void upChanged(const QVector3D &v);
    void rectChanged(const QRectF &v);
    void nearPlaneChanged(float v);
    void farPlaneChanged(float v);
    void fpsChanged(int v);
    void fpsMaxChanged(int v);
    void worldChanged(World *world);

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    void frame(void);

private:
    QVector3D m_position;
    QVector3D m_direction;
    QVector3D m_up;
    QRectF m_rect;
    float m_nearPlane;
    float m_farPlane;
    mutable CameraRenderer *m_renderer;
    int m_frameTimer;
    int m_frameCountTimer;
    int m_frameCount;
    int m_frameCountMax;
    int m_frameCountPerSecond;
    World *m_world;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_CAMERA_HPP
