//
//
#include <Volcano/Game/Manifest.hpp>

VOLCANO_GAME_BEGIN

Manifest::Manifest(QObject *parent):
    Object(parent),
    m_world(nullptr),
    m_player(nullptr),
    m_camera(nullptr),
    m_overlay(nullptr)
{
}

Manifest::Manifest(Context *context, QObject *parent):
    Object(context, parent),
    m_world(nullptr),
    m_player(nullptr),
    m_camera(nullptr),
    m_overlay(nullptr)
{
}

Manifest::~Manifest(void)
{
}

World *Manifest::world(void)
{
    return m_world;
}

void Manifest::setWorld(World *p)
{
    if (m_world != p) {
        m_world = p;
        emit worldChanged(p);
    }
}

Actor *Manifest::player(void)
{
    return m_player;
}

void Manifest::setPlayer(Actor *p)
{
    if (m_player != p) {
        m_player = p;
        emit playerChanged(p);
    }
}

Camera *Manifest::camera(void)
{
    return m_camera;
}

void Manifest::setCamera(Camera *p)
{
    if (m_camera != p) {
        m_camera = p;
        emit cameraChanged(p);
    }
}

Screen *Manifest::overlay(void)
{
    return m_overlay;
}

void Manifest::setOverlay(Screen *p)
{
    if (m_overlay != p) {
        m_overlay = p;
        emit overlayChanged(p);
    }
}

void Manifest::timerEvent(QTimerEvent *event)
{
    emit test();
}

bool Manifest::event(QEvent *event)
{
    emit test();
    qDebug() << qmlContext(this)->contextObject();
    if (Q_LIKELY(m_player != nullptr)) {
        if (m_player->event(event))
            return true;
    }

    return Object::event(event);
}

void Manifest::tick(void)
{
    if (Q_LIKELY(m_world != nullptr))
        m_world->updateState();
}

void Manifest::draw(void)
{
    auto gService = graphicsService();

    if (Q_LIKELY(m_camera != nullptr)) {
        gService->lookAt(m_camera->position(), m_camera->direction(), m_camera->up());
        //gService->perspective(m_camera->fov(), m_camera->)
    }

    if (Q_LIKELY(m_world != nullptr))
        m_world->updateGraphics();
}

VOLCANO_GAME_END
