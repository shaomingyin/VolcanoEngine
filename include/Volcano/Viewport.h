//
//
#ifndef VOLCANO_VIEWPORT_H
#define VOLCANO_VIEWPORT_H

#include <QQuickFramebufferObject>

#include <Volcano/Common.h>
#include <Volcano/Node.h>
#include <Volcano/Camera.h>

VOLCANO_BEGIN

class Viewport: public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(Node *target READ target WRITE setTarget NOTIFY targetChanged)
    Q_PROPERTY(Camera *camera READ camera WRITE setCamera NOTIFY cameraChanged)

public:
    Viewport(QQuickItem *parent = nullptr);
    ~Viewport(void) override;

public:
    Renderer *createRenderer(void) const override;

public:
    Node *target(void);
    void setTarget(Node *p);
    Camera *camera(void);
    void setCamera(Camera *p);

signals:
    void targetChanged(Node *p);
    void cameraChanged(Camera *p);

private:
    Node *m_target;
    Camera *m_camera;
};

VOLCANO_INLINE Node *Viewport::target(void)
{
    return m_target;
}

VOLCANO_INLINE Camera *Viewport::camera(void)
{
    return m_camera;
}

VOLCANO_END

#endif // VOLCANO_VIEWPORT_H
