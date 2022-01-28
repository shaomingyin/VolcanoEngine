//
//
#ifndef VOLCANO_GRAPHICS_OPENGL_QUICKSURFACE_HPP
#define VOLCANO_GRAPHICS_OPENGL_QUICKSURFACE_HPP

#include <QSize>
#include <QObject>
#include <QQuickItem>
#include <QQuickWindow>
#include <QQuickGraphicsDevice>
#include <QQuickRenderControl>
#include <QQuickRenderTarget>

#include <Volcano/Graphics/QuickSurface.hpp>
#include <Volcano/Graphics/OpenGL/Common.hpp>

VOLCANO_GRAPHICS_OPENGL_BEGIN

class QuickSurface: public Volcano::Graphics::QuickSurface {
    Q_OBJECT

public:
    QuickSurface(QObject *parent = nullptr);
    ~QuickSurface(void) override;

protected:
    QQuickGraphicsDevice *createGraphicsDevice(void) override;
    QQuickRenderTarget *createRenderTarget(const QSize &size, int sampleCount) override;
};

VOLCANO_GRAPHICS_OPENGL_END

#endif // VOLCANO_GRAPHICS_OPENGL_QUICKSURFACE_HPP
