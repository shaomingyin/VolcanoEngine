//
//
#ifndef VOLCANO_GRAPHICS_CAMERA_HPP
#define VOLCANO_GRAPHICS_CAMERA_HPP

#include <QRectF>
#include <QVector3D>
#include <QMatrix4x4>
#include <QObject>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/WorldCache.hpp>

VOLCANO_GRAPHICS_BEGIN

class Camera: public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(QVector3D up READ up WRITE setUp NOTIFY upChanged)
    Q_PROPERTY(QRectF rect READ rect WRITE setRect NOTIFY rectChanged)
    Q_PROPERTY(float nearPlane READ nearPlane WRITE setNearPlane NOTIFY nearPlaneChanged)
    Q_PROPERTY(float farPlane READ farPlane WRITE setFarPlane NOTIFY farPlaneChanged)
    Q_PROPERTY(WorldCache *worldCache READ worldCache WRITE setWorldCache NOTIFY worldCacheChanged)

public:
    Camera(QObject *parent = nullptr);
    ~Camera(void) override;

public:
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
    WorldCache *worldCache(void);
    void setWorldCache(WorldCache *worldCache);
    virtual void calcProjectMatrix(QMatrix4x4 &out);

signals:
    void positionChanged(const QVector3D &v);
    void directionChanged(const QVector3D &v);
    void upChanged(const QVector3D &v);
    void rectChanged(const QRectF &v);
    void nearPlaneChanged(float v);
    void farPlaneChanged(float v);
    void worldCacheChanged(WorldCache *worldCache);

private:
    QVector3D m_position;
    QVector3D m_direction;
    QVector3D m_up;
    QRectF m_rect;
    float m_nearPlane;
    float m_farPlane;
    WorldCache *m_worldCache;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_CAMERA_HPP
