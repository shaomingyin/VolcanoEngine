//
//
#ifndef VOLCANO_GRAPHICS_CAMERAVIEW_HPP
#define VOLCANO_GRAPHICS_CAMERAVIEW_HPP

#include <QList>
#include <QAtomicInt>
#include <QRectF>
#include <QColor>
#include <QVector3D>
#include <QMatrix4x4>
#include <QElapsedTimer>
#include <QQuickFramebufferObject>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Camera.hpp>
#include <Volcano/Graphics/ViewChain.hpp>

VOLCANO_GRAPHICS_BEGIN

class CameraView: public QQuickFramebufferObject {
    Q_OBJECT
    Q_PROPERTY(int fps READ fps NOTIFY fpsChanged)
    Q_PROPERTY(int fpsMax READ fpsMax WRITE setFpsMax NOTIFY fpsMaxChanged)
    Q_PROPERTY(bool clear READ isClear WRITE setClear NOTIFY clearChanged)
    Q_PROPERTY(QColor clearColor READ clearColor WRITE setClearColor NOTIFY clearColorChanged)
    Q_PROPERTY(Camera *camera READ camera)

public:
    CameraView(QQuickItem *parent = nullptr);
    ~CameraView(void) override;

public:
    QQuickFramebufferObject::Renderer *createRenderer(void) const override;
    int fps(void) const;
    int fpsMax(void) const;
    void setFpsMax(int v);
    bool isClear(void) const;
    void setClear(bool v);
    const QColor &clearColor(void) const;
    void setClearColor(const QColor &v);
    Camera *camera(void);

signals:
    void fpsChanged(int v);
    void fpsMaxChanged(int v);
    void clearChanged(bool v);
    void clearColorChanged(const QColor &v);

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    void frame(void);

private:
    int m_frameTimer;
    int m_frameCountTimer;
    int m_frameCount;
    int m_frameCountMax;
    int m_frameCountPerSecond;
    QElapsedTimer m_frameElapsedTimer;
    bool m_isClear;
    QColor m_clearColor;
    Camera m_camera;
    mutable ViewChain *m_viewChain;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_CAMERAVIEW_HPP
