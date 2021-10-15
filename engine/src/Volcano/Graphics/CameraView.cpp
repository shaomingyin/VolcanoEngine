//
//
#include <memory>

#include <QOpenGLContext>
#include <QQuickOpenGLUtils>

#include <Volcano/Graphics/CameraRenderer.hpp>
#include <Volcano/Graphics/CameraView.hpp>

VOLCANO_GRAPHICS_BEGIN

CameraView::CameraView(QQuickItem *parent):
    QQuickFramebufferObject(parent),
    m_isResized(false),
    m_gameWorld(nullptr),
    m_frameTimer(0),
    m_frameCount(0),
    m_frameCountPerSecond(0),
    m_frameElapsedMin(-1),
    m_position(0.0f, 0.0f, 0.0f),
    m_direction(0.0f, 0.0f, -1.0f),
    m_up(0.0f, 1.0f, 0.0f),
    m_fov(90),
    m_nearPlane(0.1f),
    m_farPlane(100.0f),
    m_isBackgroundEnabled(true),
    m_backgroundColor(QColor::fromRgb(0, 0, 0))
{
    m_frameCountTimer = startTimer(1000);

    setFpsMax(60);
}

CameraView::~CameraView(void)
{
}

CameraView::Renderer *CameraView::createRenderer(void) const
{
    return new CameraRenderer();
}

int CameraView::fps(void) const
{
    return m_frameCountPerSecond;
}

int CameraView::fpsMax(void) const
{
    return (1000 / m_frameElapsedMin);
}

void CameraView::setFpsMax(int v)
{
    if (v < 1)
        return;

    quint64 frameElapsedMin = 1000 / v;
    if (frameElapsedMin < 1)
        return;
    if (m_frameElapsedMin == frameElapsedMin)
        return;

    if (m_frameTimer > 0)
        killTimer(m_frameTimer);

    m_frameTimer = startTimer(frameElapsedMin, Qt::PreciseTimer);
    m_frameCount = 0;
    m_frameCountPerSecond = 0;
    m_elapsedTimer.restart();

    m_frameElapsedMin = frameElapsedMin;
    emit fpsMaxChanged(v);
}

const QVector3D &CameraView::position(void) const
{
    return m_position;
}

void CameraView::setPosition(const QVector3D &v)
{
    if (m_position != v) {
        m_position = v;
        emit positionChanged(v);
    }
}

const QVector3D &CameraView::direction(void) const
{
    return m_direction;
}

void CameraView::setDirection(const QVector3D &v)
{
    auto nv = v.normalized();
    if (m_direction != nv) {
        m_direction = nv;
        emit directionChanged(nv);
    }
}

const QVector3D &CameraView::up(void) const
{
    return m_up;
}

void CameraView::setUp(const QVector3D &v)
{
    auto nv = v.normalized();
    if (m_up != nv) {
        m_up = nv;
        emit upChanged(nv);
    }
}

float CameraView::fov(void) const
{
    return m_fov;
}

void CameraView::setFov(float v)
{
    if (v < 0.1f)
        return;

    if (!qFuzzyCompare(m_fov, v)) {
        m_fov = v;
        emit fovChanged(v);
    }
}

float CameraView::nearPlane(void) const
{
    return m_nearPlane;
}

void CameraView::setNearPlane(float v)
{
    if (v < 0.01f)
        return;

    if (v > m_farPlane)
        v = m_farPlane;

    if (!qFuzzyCompare(m_nearPlane, v)) {
        m_nearPlane = v;
        emit nearPlaneChanged(v);
    }
}

float CameraView::farPlane(void) const
{
    return m_farPlane;
}

void CameraView::setFarPlane(float v)
{
    if (v < 0.01f)
        return;

    if (v < m_nearPlane)
        v = m_nearPlane;

    if (!qFuzzyCompare(m_farPlane, v)) {
        m_farPlane = v;
        emit farPlaneChanged(v);
    }
}

bool CameraView::isBackgroundEnabled(void) const
{
    return m_isBackgroundEnabled;
}

void CameraView::setBackgroundEnabled(bool v)
{
    if (m_isBackgroundEnabled != v) {
        m_isBackgroundEnabled = v;
        emit backgroundEnabledChanged(v);
    }
}

const QColor CameraView::backgroundColor(void) const
{
    return m_backgroundColor;
}

void CameraView::setBackgroundColor(const QColor &v)
{
    if (m_backgroundColor != v) {
        m_backgroundColor = v;
        emit backgroundColorChanged(v);
    }
}

Game::World *CameraView::gameWorld(void)
{
    return m_gameWorld;
}

void CameraView::setGameWorld(Game::World *p)
{
    if (m_gameWorld != p) {
        m_gameWorld = p;
        emit gameWorldChanged(p);
    }
}

void CameraView::timerEvent(QTimerEvent *p)
{
    if (Q_LIKELY(p->timerId() == m_frameTimer)) {
        auto elapsed = m_elapsedTimer.restart();
        if (Q_LIKELY(isVisible()))
            update();
        m_frameCount += 1;
        return;
    }

    if (Q_LIKELY(p->timerId() == m_frameCountTimer)) {
        m_frameCountPerSecond = m_frameCount;
        m_frameCount = 0;
        emit fpsChanged(m_frameCountPerSecond);
        return;
    }
}

VOLCANO_GRAPHICS_END
