//
//
#ifndef VOLCANO_GAME_ENTITY_HPP
#define VOLCANO_GAME_ENTITY_HPP

#include <vector>

#include <Volcano/Game/Common.hpp>
#include <Volcano/Game/Component.hpp>

VOLCANO_GAME_BEGIN

using Components = std::vector<Component *>;

class Entity {
public:
    Entity(void);
    virtual ~Entity(void);

public:
    const Eigen::Vector3f &position(void) const;
    void setPosition(const Eigen::Vector3f &r);
    const Eigen::Vector3f &scale(void) const;
    void setScale(const Eigen::Vector3f &r);
    const Eigen::Quaternionf &rotation(void) const;
    void setRotation(const Eigen::Quaternionf &r);

private:
    Eigen::Vector3f m_position;
    Eigen::Vector3f m_scale;
    Eigen::Quaternionf m_rotation;
    Components m_components;
};

VOLCANO_INLINE const Eigen::Vector3f &Entity::position(void) const
{
    return m_position;
}

VOLCANO_INLINE void Entity::setPosition(const Eigen::Vector3f &r)
{
    m_position = r;
}

VOLCANO_INLINE const Eigen::Vector3f &Entity::scale(void) const
{
    return m_scale;
}

VOLCANO_INLINE void Entity::setScale(const Eigen::Vector3f &r)
{
    m_scale = r;
}

VOLCANO_INLINE const Eigen::Quaternionf &Entity::rotation(void) const
{
    return m_rotation;
}

VOLCANO_INLINE void Entity::setRotation(const Eigen::Quaternionf &r)
{
    m_rotation = r;
}

VOLCANO_GAME_END

#endif // VOLCANO_GAME_ENTITY_HPP
