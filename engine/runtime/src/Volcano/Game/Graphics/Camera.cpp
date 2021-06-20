//
//
#include <Volcano/Game/Graphics/Camera.hpp>

VOLCANO_GAME_GRAPHICS_BEGIN

Camera::Camera(QQuickItem *parent):
    QQuickFramebufferObject(parent),
    m_frameTimer(-1),
    m_frameCountTimer(-1),
    m_world(nullptr),
    m_viewRendering(1)
{
    printf("qqwer\n");
    setFpsMax(60);
}

Camera::~Camera(void)
{
}

QQuickFramebufferObject::Renderer *Camera::createRenderer(void) const
{
    printf("asdfasdfasdf");
    return new ::Volcano::Game::Graphics::Renderer();
}

const QVector3D &Camera::position(void) const
{
    return m_position;
}

void Camera::setPosition(const QVector3D &v)
{
    if (m_position != v) {
        m_position = v.normalized();
        emit positionChanged(m_position);
    }
}

const QVector3D &Camera::direction(void) const
{
    return m_direction;
}

void Camera::setDirection(const QVector3D &v)
{
    if (m_direction != v) {
        m_direction = v.normalized();
        emit directionChanged(m_direction);
    }
}

const QVector3D &Camera::up(void) const
{
    return m_up;
}

void Camera::setUp(const QVector3D &v)
{
    if (m_up != v) {
        m_up = v.normalized();
        emit upChanged(m_up);
    }
}

const QRectF &Camera::rect(void) const
{
    return m_rect;
}

void Camera::setRect(const QRectF &v)
{
    if (m_rect != v) {
        m_rect = v.normalized();
        emit rectChanged(m_rect);
    }
}

float Camera::nearPlane(void) const
{
    return m_nearPlane;
}

void Camera::setNearPlane(float v)
{
    if (!qFuzzyCompare(m_nearPlane, v)) {
        m_nearPlane = v;
        emit nearPlaneChanged(v);
    }
}

float Camera::farPlane() const
{
    return m_farPlane;
}

void Camera::setFarPlane(float v)
{
    if (!qFuzzyCompare(m_farPlane, v)) {
        m_farPlane = v;
        emit farPlaneChanged(v);
    }
}

int Camera::fps(void) const
{
    return m_frameCountPerSecond;
}

int Camera::fpsMax(void) const
{
    return m_frameCountMax;
}

void Camera::setFpsMax(int v)
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

World *Camera::world(void)
{
    return m_world;
}

void Camera::setWorld(World *world)
{
    if (m_world != world) {
        m_world = world;
        emit worldChanged(world);
    }
}

const View *Camera::lockView(void)
{
    m_viewLock.lock();
    return &m_viewFlip[m_viewRendering];
}

void Camera::unlockView(void)
{
    m_viewLock.unlock();
}

void Camera::timerEvent(QTimerEvent *event)
{
    if (Q_UNLIKELY(!isVisible()))
        return;

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

void Camera::frame(void)
{
    if (Q_LIKELY(m_world != nullptr)) {
        auto elapsed = m_frameElapsedTimer.restart();
        m_world->update(float(elapsed) / 1000.0f);
    }

    // TODO build vs...

    //m_visibleSetLock.lock();
    //m_visibleSetRendering = !m_visibleSetRendering;
    //m_visibleSetLock.unlock();

    update();
}

VOLCANO_GAME_GRAPHICS_END
