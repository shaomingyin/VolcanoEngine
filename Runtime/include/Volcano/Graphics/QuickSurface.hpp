//
//
#ifndef VOLCANO_GRAPHICS_QUICKSURFACE_HPP
#define VOLCANO_GRAPHICS_QUICKSURFACE_HPP

#include <QSize>
#include <QObject>
#include <QQuickItem>
#include <QQuickWindow>
#include <QQuickGraphicsDevice>
#include <QQuickRenderControl>
#include <QQuickRenderTarget>

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class QuickSurface: public QObject {
    Q_OBJECT

public:
    QuickSurface(QObject *parent = nullptr);
    ~QuickSurface(void) override;

public:
    bool init(QQuickWindow *window);
    void update(void);
    void render(void);

protected:
    virtual QQuickGraphicsDevice *createGraphicsDevice(void) = 0;
    virtual QQuickRenderTarget *createRenderTarget(const QSize &size, int sampleCount) = 0;

private:
    QQuickRenderControl *m_renderControl;
    QQuickRenderTarget *m_renderTarget;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_QUICKSURFACE_HPP
