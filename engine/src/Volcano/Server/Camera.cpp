//
//
#include <Volcano/Server/Camera.hpp>

VOLCANO_SERVER_BEGIN

Camera::Camera(void):
    m_direction(0.0f, 0.0f, -1.0f),
    m_up(0.0f, 1.0f, 0.0f),
    m_rect(-1.0f, 1.0f, 1.0f, -1.0f)
{
}

Camera::~Camera(void)
{
}

VOLCANO_SERVER_END
