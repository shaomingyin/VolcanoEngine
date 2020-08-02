//
//
#ifndef VOLCANO_VIEWPORT_HPP
#define VOLCANO_VIEWPORT_HPP

#include <QQuickFramebufferObject>

#include <Volcano/Common.hpp>
#include <Volcano/Scene.hpp>
#include <Volcano/Camera.hpp>

VOLCANO_BEGIN

class VOLCANO_API Viewport: public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(Scene *scene READ scene WRITE setScene NOTIFY sceneChanged)
    Q_PROPERTY(Camera *camera READ camera WRITE setCamera NOTIFY cameraChanged)

public:
    Viewport(QQuickItem *parent = nullptr);
    ~Viewport(void) override;

public:
    Renderer *createRenderer(void) const override;
    Scene *scene(void);
    void setScene(Scene *scene);
    Camera *camera(void);
    void setCamera(Camera *camera);

signals:
    void sceneChanged(void);
    void cameraChanged(void);

private:
    Scene *m_scene;
    Camera *m_camera;
};

VOLCANO_INLINE Scene *Viewport::scene(void)
{
    return m_scene;
}

VOLCANO_INLINE Camera *Viewport::camera(void)
{
    return m_camera;
}

VOLCANO_END

#endif // VOLCANO_VIEWPORT_HPP
