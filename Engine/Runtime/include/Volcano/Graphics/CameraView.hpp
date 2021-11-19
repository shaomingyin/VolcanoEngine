//
//
#ifndef VOLCANO_GRAPHICS_CAMERAVIEW_HPP
#define VOLCANO_GRAPHICS_CAMERAVIEW_HPP

#include <QColor>
#include <QVector3D>
#include <QTimerEvent>
#include <QElapsedTimer>
#include <QQuickFramebufferObject>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Camera.hpp>
#include <Volcano/Graphics/Viewable.hpp>

VOLCANO_GRAPHICS_BEGIN

class CameraView: public QQuickFramebufferObject {
    Q_OBJECT
    Q_PROPERTY(int fps READ fps NOTIFY fpsChanged)
    Q_PROPERTY(int fpsMax READ fpsMax WRITE setFpsMax NOTIFY fpsMaxChanged)
    Q_PROPERTY(Camera *camera READ camera)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(bool background READ isBackgroundEnabled WRITE setBackgroundEnabled NOTIFY backgroundEnabledChanged)
    Q_PROPERTY(Viewable *viewable READ viewable WRITE setViewable NOTIFY viewableChanged)

public:
    CameraView(QQuickItem *parent = nullptr);
    ~CameraView(void) override;

public:
    Renderer *createRenderer(void) const override;
    int fps(void) const;
    int fpsMax(void) const;
    void setFpsMax(int v);
    Camera *camera(void);
    bool isBackgroundEnabled(void) const;
    void setBackgroundEnabled(bool v);
    const QColor backgroundColor(void) const;
    void setBackgroundColor(const QColor &v);
    Viewable *viewable(void);
    void setViewable(Viewable *p);

signals:
    void fpsChanged(int v);
    void fpsMaxChanged(int v);
    void backgroundEnabledChanged(bool v);
    void backgroundColorChanged(const QColor &v);
    void viewableChanged(Viewable *p);

protected:
    void timerEvent(QTimerEvent *p) override;

private:
    void frame(quint64 elapsed);

private:
    bool m_isResized;
    bool m_isBackgroundEnabled;
    QColor m_backgroundColor;
    Viewable *m_viewable;
    QElapsedTimer m_elapsedTimer;
    int m_frameTimer;
    int m_frameCountTimer;
    long m_frameCount;
    long m_frameCountPerSecond;
    quint64 m_frameElapsedMin;
    Camera m_camera;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_CAMERAVIEW_HPP
