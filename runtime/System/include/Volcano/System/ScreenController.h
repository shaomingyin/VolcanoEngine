//
//
#ifndef VOLCANO_SYSTEM_SCREENCONTROLLER_H
#define VOLCANO_SYSTEM_SCREENCONTROLLER_H

#include <memory>

#include <QEvent>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QOpenGLContext>
#include <QOpenGLFramebufferObject>
#include <QQuickRenderControl>
#include <QQuickWindow>
#include <QCoreApplication>
#include <QObject>

#include <Volcano/World/Screen.h>
#include <Volcano/Graphics/View.h>
#include <Volcano/System/Common.h>

VOLCANO_SYSTEM_BEGIN

class ScreenController: public QObject {
    Q_OBJECT

public:
    ScreenController(World::Screen& screen, QObject* parent = nullptr);

public:
    void feedEvent(QEvent* e) {
        QCoreApplication::sendEvent(window_.get(), e);
    }

    void update();
    void render(Graphics::View& view);

private slots:
    void onScreenStatusChanged(World::Screen::Status v);
    void onScreenSizeChanged(const QSize& v);

private:
    void setupRenderTarget();

private:
    World::Screen& screen_;
    std::unique_ptr<QQuickRenderControl> render_control_;
    std::unique_ptr<QQuickWindow> window_;
    std::unique_ptr<QOpenGLFramebufferObject> frame_buffer_;

#ifdef VOLCANO_DEBUG
    QOpenGLContext* gl_context_;
#endif
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_SCREENCONTROLLER_H
