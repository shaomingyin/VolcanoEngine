//
//
#include <memory>

#include <QMutexLocker>

#include <Volcano/Game/Graphics/Camera.hpp>

VOLCANO_GAME_GRAPHICS_BEGIN

Camera::Camera(QQuickItem *parent):
    QQuickFramebufferObject(parent),
    m_isClear(true),
    m_clearColor(Qt::black),
    m_frameTimer(-1),
    m_frameCountTimer(-1),
    m_world(nullptr),
    m_viewRendering(1)
{
    setFpsMax(60);
}

Camera::~Camera(void)
{
}

QQuickFramebufferObject::Renderer *Camera::createRenderer(void) const
{
    auto renderer = std::make_unique<::Volcano::Game::Graphics::Renderer>(const_cast<Camera &>(*this));
    if (!renderer || !renderer->init())
        return nullptr;
    return renderer.release();
}

bool Camera::isClear(void) const
{
    return m_isClear;
}

void Camera::setClear(bool v)
{
    if (m_isClear != v) {
        m_isClear = v;
        emit clearChanged(v);
    }
}

const QColor &Camera::clearColor(void) const
{
    return m_clearColor;
}

void Camera::setClearColor(const QColor &v)
{
    if (m_clearColor != v) {
        m_clearColor = v;
        emit clearColorChanged(v);
    }
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
    if (m_viewState.testAndSetRelaxed(2, 3))
        return &m_viewFlip[m_viewRendering];
    return nullptr;
}

void Camera::unlockView(void)
{
    Q_ASSERT(m_viewState == 3);
    m_viewState = 0;
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
    if (Q_UNLIKELY(!isVisible() || size().isEmpty()))
        return;

    // TODO make sure the renderer has initialized.

    auto &view = m_viewFlip[!m_viewRendering];
    buildView(view);

    while (!m_viewState.testAndSetRelaxed(0, 1) && !m_viewState.testAndSetRelaxed(2, 1));

    m_viewRendering = !m_viewRendering;
    m_viewState = 2;

    update();
}

void Camera::buildView(View &out)
{
    // out.clear();

    if (Q_UNLIKELY(m_world == nullptr))
        return;

    // TODO add visible object to 'out'...
}

VOLCANO_GAME_GRAPHICS_END
