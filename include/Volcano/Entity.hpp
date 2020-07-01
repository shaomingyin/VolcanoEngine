//
//
#ifndef VOLCANO_ENTITY_HPP
#define VOLCANO_ENTITY_HPP

#include <Volcano/Common.hpp>
#include <Volcano/Node.hpp>

VOLCANO_BEGIN

class Entity : public Node {
public:
	Entity(Node *parent = nullptr);
	~Entity(void) override;

public:
	const glm::vec3 &position(void) const;
	void setPosition(const glm::vec3 &v);
	const glm::vec3 &scale(void) const;
	void setScale(const glm::vec3 &v);
	const glm::quat &rotation(void) const;
	void setRotation(const glm::quat &v);
	glm::mat4 transform(void) const;

private:
	glm::vec3 m_position;
	glm::vec3 m_scale;
	glm::quat m_rotation;
};

VOLCANO_INLINE const glm::vec3 &Entity::position(void) const
{
	return m_position;
}

VOLCANO_INLINE void Entity::setPosition(const glm::vec3 &v)
{
	m_position = v;
}

VOLCANO_INLINE const glm::vec3 &Entity::scale(void) const
{
	return m_scale;
}

VOLCANO_INLINE void Entity::setScale(const glm::vec3 &v)
{
	m_scale = v;
}

VOLCANO_INLINE const glm::quat &Entity::rotation(void) const
{
	return m_rotation;
}

VOLCANO_INLINE void Entity::setRotation(const glm::quat &v)
{
	m_rotation = v;
}

VOLCANO_INLINE glm::mat4 Entity::transform(void) const
{
	glm::mat4 m(1.0f);
	m = glm::rotate(m, glm::angle(m_rotation), glm::axis(m_rotation));
	m = glm::scale(m, m_scale);
	return glm::translate(m, m_position);
}

VOLCANO_END

#endif // VOLCANO_ENTITY_HPP
