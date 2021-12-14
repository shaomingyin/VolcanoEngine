//
//
#ifndef VOLCANO_GRAPHICS_CAMERAVIEW_HPP
#define VOLCANO_GRAPHICS_CAMERAVIEW_HPP

#include <QColor>
#include <QVector3D>
#include <QTimerEvent>
#include <QElapsedTimer>
#include <QQuickFramebufferObject>

#include <Volcano/Game/WorldBase.hpp>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/World.hpp>
#include <Volcano/Graphics/Camera.hpp>
#include <Volcano/Graphics/View.hpp>

VOLCANO_GRAPHICS_BEGIN

class CameraView: public QQuickFramebufferObject {
    Q_OBJECT
    Q_PROPERTY(bool activate READ isActivate WRITE setActivate NOTIFY activateChanged)
    Q_PROPERTY(int fps READ fps NOTIFY fpsChanged)
    Q_PROPERTY(int fpsMax READ fpsMax WRITE setFpsMax NOTIFY fpsMaxChanged)
    Q_PROPERTY(Camera *camera READ camera)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(bool background READ isBackgroundEnabled WRITE setBackgroundEnabled NOTIFY backgroundEnabledChanged)
    Q_PROPERTY(Game::WorldBase *gameWorld READ gameWorld WRITE setGameWorld NOTIFY gameWorldChanged)

public:
    CameraView(QQuickItem *parent = nullptr);
    ~CameraView(void) override;

public:
    Renderer *createRenderer(void) const override;
    bool isActivate(void) const;
    void setActivate(bool v);
    int fps(void) const;
    int fpsMax(void) const;
    void setFpsMax(int v);
    Camera *camera(void);
    bool isBackgroundEnabled(void) const;
    void setBackgroundEnabled(bool v);
    const QColor backgroundColor(void) const;
    void setBackgroundColor(const QColor &v);
    Game::WorldBase *gameWorld(void);
    void setGameWorld(Game::WorldBase *p);
    void buildView(View *view);

signals:
    void activateChanged(bool v);
    void fpsChanged(int v);
    void fpsMaxChanged(int v);
    void backgroundEnabledChanged(bool v);
    void backgroundColorChanged(const QColor &v);
    void gameWorldChanged(Game::WorldBase *p);

protected:
    void timerEvent(QTimerEvent *p) override;

private:
    void frame(Duration elapsed);

private:
    bool m_isActivate;
    bool m_isBackgroundEnabled;
    QColor m_backgroundColor;
    QElapsedTimer m_elapsedTimer;
    int m_frameTimer;
    int m_frameCountTimer;
    long m_frameCount;
    long m_frameCountPerSecond;
    quint64 m_frameElapsedMin;
    Camera m_camera;
    Game::WorldBase *m_gameWorld;
    World m_world;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_CAMERAVIEW_HPP
