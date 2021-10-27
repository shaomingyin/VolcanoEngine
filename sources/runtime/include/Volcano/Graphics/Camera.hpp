//
//
#ifndef VOLCANO_GRAPHICS_CAMERA_HPP
#define VOLCANO_GRAPHICS_CAMERA_HPP

#include <QColor>
#include <QVector3D>
#include <QObject>

#include <Volcano/Game/Entity.hpp>
#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class Camera: public QObject {
    Q_OBJECT
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(QVector3D up READ up WRITE setUp NOTIFY upChanged)
    Q_PROPERTY(float fov READ fov WRITE setFov NOTIFY fovChanged)
    Q_PROPERTY(float nearPlane READ nearPlane WRITE setNearPlane NOTIFY nearPlaneChanged)
    Q_PROPERTY(float farPlane READ farPlane WRITE setFarPlane NOTIFY farPlaneChanged)
    Q_PROPERTY(Game::Entity *gameEntity READ gameEntity WRITE setGameEntity NOTIFY gameEntityChanged)

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
    float fov(void) const;
    void setFov(float v);
    float nearPlane(void) const;
    void setNearPlane(float v);
    float farPlane(void) const;
    void setFarPlane(float v);
    Game::Entity *gameEntity(void);
    void setGameEntity(Game::Entity *p);

signals:
    void positionChanged(const QVector3D &v);
    void directionChanged(const QVector3D &v);
    void upChanged(const QVector3D &v);
    void fovChanged(float v);
    void nearPlaneChanged(float v);
    void farPlaneChanged(float v);
    void gameEntityChanged(Game::Entity *p);

private slots:
    void onGameEntityPositionChanged(const QVector3D &v);
    void onGameEntityRotationChanged(const QQuaternion &v);

private:
    QVector3D m_position;
    QVector3D m_direction;
    QVector3D m_up;
    float m_fov;
    float m_nearPlane;
    float m_farPlane;
    Game::Entity *m_gameEntity;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_CAMERAVIEW_HPP
