//
//
#include <Volcano/Camera.hpp>

VOLCANO_BEGIN

Camera::Camera(Node *parent) :
	Node(parent)
{
}

Camera::~Camera(void)
{
}

void Camera::setViewport(const glm::vec4 &v)
{
	VOLCANO_ASSERT(v[0] < v[1]);
	VOLCANO_ASSERT(v[2] < v[3]);

	m_viewport = v;

	switch (m_type) {
	case TypeOrtho:
		setOrtho(m_near, m_far);
		break;
	case TypePerspective:
		setPerspective(m_fov, m_near, m_far);
		break;
	}
}

void Camera::setOrtho(float near, float far)
{
	VOLCANO_ASSERT(near < far);

	m_projection = glm::ortho(m_viewport[0], m_viewport[1], m_viewport[2], m_viewport[3], near, far);
	m_near = near;
	m_far = far;
	m_type = TypeOrtho;
}

void Camera::setPerspective(float fov, float near, float far)
{
	VOLCANO_ASSERT(near < far);

	m_fov = glm::clamp(fov, 1.0f, 179.0f);
	m_projection = glm::perspective(m_fov, (m_viewport[1] - m_viewport[0]) / (m_viewport[3] - m_viewport[2]), near, far);
	m_near = near;
	m_far = far;
	m_type = TypePerspective;
}

VOLCANO_END
