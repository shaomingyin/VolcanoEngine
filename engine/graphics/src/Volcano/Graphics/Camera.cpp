//
//
#include <memory>

#include <QPointer>
#include <QMutexLocker>

#include <Volcano/Graphics/Camera.hpp>

VOLCANO_GRAPHICS_BEGIN

Camera::Camera(QQuickItem *parent):
    QQuickFramebufferObject(parent),
    m_frameTimer(-1),
    m_frameCountTimer(-1),
    m_isClear(true),
    m_clearColor(Qt::black),
    m_isPerpective(false),
    m_fov(90.0f),
    m_gameWorld(nullptr),
    m_vsRendering(1)
{
    setFpsMax(60);
}

Camera::~Camera(void)
{
}

QQuickFramebufferObject::Renderer *Camera::createRenderer(void) const
{
    auto renderer = std::make_unique<::Volcano::Graphics::Renderer>(const_cast<Camera &>(*this));
    if (!renderer || !renderer->init())
        return nullptr;
    return renderer.release();
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

bool Camera::isPerpective(void) const
{
    return m_isPerpective;
}

void Camera::setPerpective(bool v)
{
    if (m_isPerpective != v) {
        m_isPerpective = v;
        emit perpectiveChanged(v);
    }
}

qreal Camera::fov(void) const
{
    return m_fov;
}

void Camera::setFov(qreal v)
{
    qreal tmp = qBound(1.0f, v, 179.0f);
    if (!qFuzzyCompare(m_fov, tmp)) {
        m_fov = tmp;
        emit fovChanged(tmp);
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

Game::World *Camera::gameWorld(void)
{
    return m_gameWorld;
}

void Camera::setGameWorld(Game::World *gameWorld)
{
    if (m_gameWorld == gameWorld)
        return;

    reset();

    m_gameWorld = gameWorld;

    if (m_gameWorld != nullptr) {
        auto &gameObjects = gameWorld->objects();
        for (auto gameObject: gameObjects)
            addGameObject(gameObject);
        connect(m_gameWorld, &Game::World::objectAdded, this, &Camera::addGameObject);
        connect(m_gameWorld, &Game::World::objectRemoved, this, &Camera::removeGameObject);
    }

    emit gameWorldChanged(gameWorld);
}

const VisibleSet *Camera::lockVisibleSet(void)
{
    if (m_vsState.testAndSetRelaxed(2, 3))
        return &m_vsFlip[m_vsRendering];
    return nullptr;
}

void Camera::unlockVisibleSet(void)
{
    Q_ASSERT(m_vsState == 3);
    m_vsState = 0;
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

QSGNode *Camera::updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *updatePaintNodeData)
{
    return QQuickFramebufferObject::updatePaintNode(oldNode, updatePaintNodeData);
}

void Camera::frame(void)
{
    if (Q_UNLIKELY(!isVisible() || size().isEmpty()))
        return;

    // TODO make sure the renderer has initialized.

    auto &vs = m_vsFlip[!m_vsRendering];
    buildVisibleSet(vs);

    while (!m_vsState.testAndSetRelaxed(0, 1) && !m_vsState.testAndSetRelaxed(2, 1));

    m_vsRendering = !m_vsRendering;
    m_vsState = 2;

    update();
}

void Camera::buildVisibleSet(VisibleSet &out)
{
    out.reset();

    if (Q_UNLIKELY(m_gameWorld == nullptr))
        return;

    // TODO add visible object to 'out'...
}

void Camera::reset(void)
{
}

void Camera::addGameObject(Game::Object *gameObject)
{
    Q_ASSERT(gameObject != nullptr);

    auto gameEntity = qobject_cast<Game::Entity *>(gameObject);
    if (gameEntity != nullptr) {
        addGameEntity(gameEntity);
        return;
    }

    auto gameSpotLight = qobject_cast<Game::SpotLight *>(gameObject);
    if (gameSpotLight != nullptr) {
        addGameSpotLight(gameSpotLight);
        return;
    }

    auto gameDirectionalLight = qobject_cast<Game::DirectionalLight *>(gameObject);
    if (gameDirectionalLight != nullptr) {
        addGameDirectionalLight(gameDirectionalLight);
        return;
    }

    auto gamePointLight = qobject_cast<Game::PointLight *>(gameObject);
    if (gamePointLight != nullptr) {
        addGamePointLight(gamePointLight);
        return;
    }
}

void Camera::addGameEntity(Game::Entity *gameEntity)
{
    Q_ASSERT(gameEntity != nullptr);

    auto gameComponents = gameEntity->components();
    for (auto gameComponent: gameComponents) {
        auto gameMesh = qobject_cast<Game::Mesh *>(gameComponent);
        if (gameMesh != nullptr) {
            addGameMesh(gameMesh);
            continue;
        }
    }
}

void Camera::addGameMesh(Game::Mesh *gameMesh)
{
    Q_ASSERT(gameMesh != nullptr);

    m_meshListPengine.append(gameMesh);
}

void Camera::addGameDirectionalLight(Game::DirectionalLight *gameDirectionalLight)
{
}

void Camera::addGamePointLight(Game::PointLight *gamePointLight)
{
}

void Camera::addGameSpotLight(Game::SpotLight *gameSpotLight)
{
}

void Camera::removeGameObject(Game::Object *gameObject)
{
    auto gameEntity = qobject_cast<Game::Entity *>(gameObject);
    if (gameEntity != nullptr) {
        removeGameEntity(gameEntity);
        return;
    }
}

void Camera::removeGameEntity(Game::Entity *gameEntity)
{

}

VOLCANO_GRAPHICS_END
