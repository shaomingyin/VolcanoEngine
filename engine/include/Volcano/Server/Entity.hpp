//
//
#ifndef VOLCANO_SERVER_ENTITY_HPP
#define VOLCANO_SERVER_ENTITY_HPP

#include <vector>
#include <string>

#include <Volcano/Server/Common.hpp>
#include <Volcano/Server/Object.hpp>
#include <Volcano/Server/Component.hpp>

VOLCANO_SERVER_BEGIN

using Components = std::vector<Component *>;

class Entity: public Object {
public:
    Entity(void);
    virtual ~Entity(void);

public:
    const glm::vec3 &position(void) const;
    void setPosition(const glm::vec3 &r);
    const glm::vec3 &scale(void) const;
    void setScale(const glm::vec3 &r);
    const glm::quat &rotation(void) const;
    void setRotation(const glm::quat &r);

private:
    glm::vec3 m_position;
    glm::vec3 m_scale;
    glm::quat m_rotation;
    Components m_components;
};

VOLCANO_INLINE const glm::vec3 &Entity::position(void) const
{
    return m_position;
}

VOLCANO_INLINE void Entity::setPosition(const glm::vec3 &r)
{
    m_position = r;
}

VOLCANO_INLINE const glm::vec3 &Entity::scale(void) const
{
    return m_scale;
}

VOLCANO_INLINE void Entity::setScale(const glm::vec3 &r)
{
    m_scale = r;
}

VOLCANO_INLINE const glm::quat &Entity::rotation(void) const
{
    return m_rotation;
}

VOLCANO_INLINE void Entity::setRotation(const glm::quat &r)
{
    m_rotation = r;
}

VOLCANO_SERVER_END

#endif // VOLCANO_SERVER_ENTITY_HPP
