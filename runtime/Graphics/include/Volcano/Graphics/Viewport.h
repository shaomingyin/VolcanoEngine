//
//
#ifndef VOLCANO_GRAPHICS_VIEWPORT_H
#define VOLCANO_GRAPHICS_VIEWPORT_H

#include <QQuickFramebufferObject>

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Camera.h>

VOLCANO_GRAPHICS_BEGIN

class Viewport: public QQuickFramebufferObject {
    Q_OBJECT
    Q_PROPERTY(Camera* camera READ camera WRITE setCamera NOTIFY cameraChanged FINAL)

public:
    Viewport(QQuickItem* parent = nullptr);

public:
    Camera* camera() {
        return camera_;
    }

    void setCamera(Camera* p) {
        if (camera_ != p) {
            camera_ = p;
            emit cameraChanged(p);
        }
    }

    Renderer *createRenderer() const override;

signals:
    void cameraChanged(Camera* p);

private:
    Camera* camera_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_VIEWPORT_H
