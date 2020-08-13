//
//
#ifndef VOLCANO_VIEWPORT_HPP
#define VOLCANO_VIEWPORT_HPP

#include <QTimerEvent>
#include <QQuickFramebufferObject>

#include <Volcano/Common.hpp>
#include <Volcano/Node.hpp>
#include <Volcano/Camera.hpp>

VOLCANO_BEGIN

class VOLCANO_API Viewport: public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(Node *target READ target WRITE setTarget NOTIFY targetChanged)
    Q_PROPERTY(Camera *camera READ camera WRITE setCamera NOTIFY cameraChanged)
    Q_PROPERTY(int fps READ fps WRITE setFps NOTIFY fpsChanged)
    Q_PROPERTY(bool showCameras READ showCameras WRITE setShowCameras NOTIFY showCamerasChanged)

public:
    Viewport(QQuickItem *parent = nullptr);
    ~Viewport(void) override;

public:
    Renderer *createRenderer(void) const override;
    Node *target(void) const;
    void setTarget(Node *node);
    Camera *camera(void) const;
    void setCamera(Camera *camera);
    int fps(void) const;
    void setFps(int v);
    bool showCameras(void) const;
    void setShowCameras(bool v);

signals:
    void targetChanged(void);
    void cameraChanged(void);
    void fpsChanged(void);
    void showCamerasChanged(void);

protected:
    void timerEvent(QTimerEvent *event) override;
    void frame(void);

private:
    Node *m_target;
    Camera *m_camera;
    int m_fps;
    int m_frameTimer;
    bool m_showCameras;
};

VOLCANO_INLINE Node *Viewport::target(void) const
{
    return m_target;
}

VOLCANO_INLINE Camera *Viewport::camera(void) const
{
    return m_camera;
}

VOLCANO_INLINE int Viewport::fps(void) const
{
    return m_fps;
}

VOLCANO_INLINE bool Viewport::showCameras(void) const
{
    return m_showCameras;
}

VOLCANO_END

#endif // VOLCANO_VIEWPORT_HPP
