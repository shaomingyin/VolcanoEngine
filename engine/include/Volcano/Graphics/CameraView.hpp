//
//
#ifndef VOLCANO_GRAPHICS_CAMERAVIEW_HPP
#define VOLCANO_GRAPHICS_CAMERAVIEW_HPP

#include <QColor>
#include <QVector3D>
#include <QTimerEvent>
#include <QElapsedTimer>
#include <QQuickFramebufferObject>

#include <Volcano/Game/World.hpp>
#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class CameraView: public QQuickFramebufferObject {
    Q_OBJECT
    Q_PROPERTY(int fps READ fps NOTIFY fpsChanged)
    Q_PROPERTY(int fpsMax READ fpsMax WRITE setFpsMax NOTIFY fpsMaxChanged)
    Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(QVector3D direction READ direction WRITE setDirection NOTIFY directionChanged)
    Q_PROPERTY(QVector3D up READ up WRITE setUp NOTIFY upChanged)
    Q_PROPERTY(float fov READ fov WRITE setFov NOTIFY fovChanged)
    Q_PROPERTY(float nearPlane READ nearPlane WRITE setNearPlane NOTIFY nearPlaneChanged)
    Q_PROPERTY(float farPlane READ farPlane WRITE setFarPlane NOTIFY farPlaneChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(bool enableBackground READ isBackgroundEnabled WRITE setBackgroundEnabled NOTIFY backgroundEnabledChanged)
    Q_PROPERTY(Game::World *gameWorld READ gameWorld WRITE setGameWorld NOTIFY gameWorldChanged)

public:
    CameraView(QQuickItem *parent = nullptr);
    ~CameraView(void) override;

public:
    Renderer *createRenderer(void) const override;
    int fps(void) const;
    int fpsMax(void) const;
    void setFpsMax(int v);
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
    bool isBackgroundEnabled(void) const;
    void setBackgroundEnabled(bool v);
    const QColor backgroundColor(void) const;
    void setBackgroundColor(const QColor &v);
    Game::World *gameWorld(void);
    void setGameWorld(Game::World *p);

signals:
    void fpsChanged(int v);
    void fpsMaxChanged(int v);
    void positionChanged(const QVector3D &v);
    void directionChanged(const QVector3D &v);
    void upChanged(const QVector3D &v);
    void fovChanged(float v);
    void nearPlaneChanged(float v);
    void farPlaneChanged(float v);
    void backgroundEnabledChanged(bool v);
    void backgroundColorChanged(const QColor &v);
    void gameWorldChanged(Game::World *p);

protected:
    void timerEvent(QTimerEvent *p) override;

private:
    void frame(quint64 elapsed);
    void renderGameWorld(void);

private:
    bool m_isResized;
    bool m_isBackgroundEnabled;
    QColor m_backgroundColor;
    Game::World *m_gameWorld;
    QElapsedTimer m_elapsedTimer;
    int m_frameTimer;
    int m_frameCountTimer;
    long m_frameCount;
    long m_frameCountPerSecond;
    quint64 m_frameElapsedMin;
    QVector3D m_position;
    QVector3D m_direction;
    QVector3D m_up;
    float m_fov;
    float m_nearPlane;
    float m_farPlane;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_CAMERAVIEW_HPP
