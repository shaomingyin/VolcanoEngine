//
//
#include <Volcano/Game/Camera.hpp>

VOLCANO_GAME_BEGIN

Camera::Camera(QObject *parent):
    QObject(parent),
    m_direction(0.0f, 0.0f, -1.0f),
    m_up(0.0f, 1.0f, 0.0f),
    m_rect(-1.0f, 1.0f, 1.0f, -1.0f)
{
}

Camera::~Camera(void)
{
}

VOLCANO_GAME_END
