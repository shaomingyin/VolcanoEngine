//
//
#ifndef VOLCANO_GRAPHICS_WORLDVIEW_HPP
#define VOLCANO_GRAPHICS_WORLDVIEW_HPP

#include <QTimerEvent>
#include <QQuickFramebufferObject>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Camera.hpp>

VOLCANO_GRAPHICS_BEGIN

class WorldView: public QQuickFramebufferObject {
    Q_OBJECT
    Q_PROPERTY(Camera *camera READ camera WRITE setCamera NOTIFY cameraChanged)
    Q_PROPERTY(int fps READ fps NOTIFY fpsChanged)
    Q_PROPERTY(int fpsMax READ fpsMax WRITE setFpsMax NOTIFY fpsMaxChanged)

public:
    WorldView(QQuickItem *parent = nullptr);
    ~WorldView(void) override;

public:
    Renderer *createRenderer(void) const override;
    Camera *camera(void);
    void setCamera(Camera *camera);
    int fps(void) const;
    int fpsMax(void) const;
    void setFpsMax(int v);

signals:
    void cameraChanged(Camera *camera);
    void fpsChanged(int v);
    void fpsMaxChanged(int v);

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    mutable Renderer *m_worldRenderer;
    Camera *m_camera;
    int m_frameTimer;
    int m_frameCountTimer;
    int m_frameCount;
    int m_frameCountMax;
    int m_frameCountPerSecond;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_WORLDVIEW_HPP
