//
//
#ifndef VOLCANO_VM_COMPONENT_HPP
#define VOLCANO_VM_COMPONENT_HPP

#include <Volcano/VM/Common.hpp>
#include <Volcano/VM/Object.hpp>

VOLCANO_VM_BEGIN

class Component: public Object {
public:
    Component(void);
    ~Component(void) override;
};

VOLCANO_VM_END

#endif // VOLCANO_VM_COMPONENT_HPP
