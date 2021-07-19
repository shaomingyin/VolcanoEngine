//
//
#include <memory>

#include <QPointer>
#include <QRunnable>
#include <QMutexLocker>

#include <Volcano/Graphics/Camera.hpp>

VOLCANO_GRAPHICS_BEGIN

Camera::Camera(QObject *parent):
    QObject(parent),
    m_isPerpective(false),
    m_fov(90.0f),
    m_gameWorld(nullptr)
{
}

Camera::~Camera(void)
{
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

void Camera::buildView(View *out)
{
    Q_ASSERT(out != nullptr);

    out->reset();

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

    m_addedGameObjectList.append(gameObject);
}

void Camera::addGameEntity(Game::Entity *gameEntity)
{
    Q_ASSERT(gameEntity != nullptr);

    m_entityList.emplaceBack(gameEntity);
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
