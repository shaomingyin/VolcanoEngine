//
//
#ifndef VOLCANO_VM_ENTITY_HPP
#define VOLCANO_VM_ENTITY_HPP

#include <vector>
#include <string>

#include <Volcano/VM/Common.hpp>
#include <Volcano/VM/Component.hpp>

VOLCANO_VM_BEGIN

using Components = std::vector<Component *>;

class Entity: public Noncopyable {
public:
    Entity(void);
    virtual ~Entity(void);

private:
    Components m_components;
};

VOLCANO_VM_END

#endif // VOLCANO_VM_ENTITY_HPP
