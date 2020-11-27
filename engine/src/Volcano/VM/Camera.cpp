//
//
#include <Volcano/VM/Camera.hpp>

VOLCANO_VM_BEGIN

Camera::Camera(void):
    m_direction(0.0f, 0.0f, -1.0f),
    m_up(0.0f, 1.0f, 0.0f),
    m_rect(-1.0f, 1.0f, 1.0f, -1.0f)
{
}

Camera::~Camera(void)
{
}

void Camera::applyToView(Graphics::View &view)
{
    view.setPosition(m_position);
    view.setDirection(m_direction);
    view.setUp(m_up);
    view.setRect(m_rect);
}

VOLCANO_VM_END
