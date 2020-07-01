//
//
#ifndef VOLCANO_CAMERA_HPP
#define VOLCANO_CAMERA_HPP

#include <Volcano/Common.hpp>
#include <Volcano/Node.hpp>

VOLCANO_BEGIN

class Camera : public Node {
public:
	enum Type {
		TypeOrtho = 0,
		TypePerspective,
	};

public:
	Camera(Node *parent = nullptr);
	~Camera(void) override;

public:
	Type type(void) const;
	const glm::vec4 &viewport(void) const;
	void setViewport(const glm::vec4 &v);
	float near(void) const;
	float far(void) const;
	void setOrtho(float near, float far);
	float fov(void) const;
	void setPerspective(float fov, float near, float far);
	const glm::mat4 &projection(void) const;
	const glm::vec3 &position(void) const;
	const glm::vec3 &direction(void) const;
	const glm::vec3 &up(void) const;
	void lookAt(const glm::vec3 &postion, const glm::vec3 &direction, const glm::vec3 &up);
	glm::mat4 viewMatrix(void) const;

private:
	Type m_type;
	glm::vec4 m_viewport;
	float m_fov;
	float m_near;
	float m_far;
	glm::vec3 m_position;
	glm::vec3 m_direction;
	glm::vec3 m_up;
	glm::mat4 m_projection;
};

VOLCANO_INLINE Camera::Type Camera::type(void) const
{
	return m_type;
}

VOLCANO_INLINE const glm::vec4 &Camera::viewport(void) const
{
	return m_viewport;
}

VOLCANO_INLINE float Camera::near(void) const
{
	return m_near;
}

VOLCANO_INLINE float Camera::far(void) const
{
	return m_far;
}

VOLCANO_INLINE float Camera::fov(void) const
{
	VOLCANO_ASSERT(m_type == TypePerspective);
	return m_fov;
}

VOLCANO_INLINE const glm::mat4 &Camera::projection(void) const
{
	return m_projection;
}

VOLCANO_INLINE const glm::vec3 &Camera::direction(void) const
{
	return m_direction;
}

VOLCANO_INLINE const glm::vec3 &Camera::up(void) const
{
	return m_up;
}

VOLCANO_INLINE void Camera::lookAt(const glm::vec3 &position, const glm::vec3 &direction, const glm::vec3 &up)
{
	m_position = position;
	m_direction = glm::normalize(direction);
	m_up = glm::normalize(up);
}

VOLCANO_INLINE glm::mat4 Camera::viewMatrix(void) const
{
	return glm::lookAt(m_position, m_direction, m_up);
}

VOLCANO_END

#endif // VOLCANO_CAMERA_HPP
