//
//
#include <Volcano/CameraView.hpp>

VOLCANO_BEGIN

CameraView::CameraView(QQuickItem *parent):
    QQuickFramebufferObject(parent),
    m_frameTimer(-1),
    m_frameCountTimer(-1),
    m_world(nullptr),
    m_renderer(nullptr),
    m_visibleSetRendering(1)
{
    setFpsMax(60);
}

CameraView::~CameraView(void)
{
}

QQuickFramebufferObject::Renderer *CameraView::createRenderer(void) const
{
    m_renderer = new CameraRenderer();

    return m_renderer;
}

const QVector3D &CameraView::position(void) const
{
    return m_position;
}

void CameraView::setPosition(const QVector3D &v)
{
    if (m_position != v) {
        m_position = v.normalized();
        emit positionChanged(m_position);
    }
}

const QVector3D &CameraView::direction(void) const
{
    return m_direction;
}

void CameraView::setDirection(const QVector3D &v)
{
    if (m_direction != v) {
        m_direction = v.normalized();
        emit directionChanged(m_direction);
    }
}

const QVector3D &CameraView::up(void) const
{
    return m_up;
}

void CameraView::setUp(const QVector3D &v)
{
    if (m_up != v) {
        m_up = v.normalized();
        emit upChanged(m_up);
    }
}

const QRectF &CameraView::rect(void) const
{
    return m_rect;
}

void CameraView::setRect(const QRectF &v)
{
    if (m_rect != v) {
        m_rect = v.normalized();
        emit rectChanged(m_rect);
    }
}

float CameraView::nearPlane(void) const
{
    return m_nearPlane;
}

void CameraView::setNearPlane(float v)
{
    if (!qFuzzyCompare(m_nearPlane, v)) {
        m_nearPlane = v;
        emit nearPlaneChanged(v);
    }
}

float CameraView::farPlane() const
{
    return m_farPlane;
}

void CameraView::setFarPlane(float v)
{
    if (!qFuzzyCompare(m_farPlane, v)) {
        m_farPlane = v;
        emit farPlaneChanged(v);
    }
}

int CameraView::fps(void) const
{
    return m_frameCountPerSecond;
}

int CameraView::fpsMax(void) const
{
    return m_frameCountMax;
}

void CameraView::setFpsMax(int v)
{
    if (m_frameCountMax == v)
        return;

    if (m_frameTimer >= 0) {
        killTimer(m_frameTimer);
        m_frameTimer = -1;
    }

    m_frameCountMax = v;
    if (m_frameCountMax < 1)
        m_frameCount = 1;

    m_frameCount = 0;
    m_frameCountPerSecond = 0;
    m_frameTimer = startTimer(1000 / v, Qt::PreciseTimer);

    m_frameElapsedTimer.restart();

    if (m_frameCountTimer >= 0) {
        killTimer(m_frameCountTimer);
        m_frameCountTimer = -1;
    }

    m_frameCountTimer = startTimer(1000, Qt::PreciseTimer);

    emit fpsMaxChanged(v);
}

World *CameraView::world(void)
{
    return m_world;
}

void CameraView::setWorld(World *world)
{
    if (m_world != world) {
        m_world = world;
        emit worldChanged(world);
    }
}

const VisibleSet *CameraView::lockVisibleSet(void)
{
    m_visibleSetLock.lock();
    return &m_visibleSetFlip[m_visibleSetRendering];
}

void CameraView::unlockVisibleSet(void)
{
    m_visibleSetLock.unlock();
}

void CameraView::timerEvent(QTimerEvent *event)
{
    if (Q_LIKELY(event->timerId() == m_frameTimer)) {
        frame();
        m_frameCount += 1;
        return;
    }

    if (event->timerId() == m_frameCountTimer) {
        m_frameCountPerSecond = m_frameCount;
        m_frameCount = 0;
        emit fpsChanged(m_frameCountPerSecond);
        return;
    }
}

void CameraView::frame(void)
{
    if (Q_LIKELY(m_world != nullptr)) {
        auto elapsed = m_frameElapsedTimer.restart();
        m_world->update(float(elapsed) / 1000.0f);
    }

    if (Q_LIKELY(m_renderer == nullptr)) {
        // TODO build vs...

        m_visibleSetLock.lock();
        m_visibleSetRendering = !m_visibleSetRendering;
        m_visibleSetLock.unlock();

        update();
    }
}

VOLCANO_END
