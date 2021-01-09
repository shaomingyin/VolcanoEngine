//
//
#ifndef VOLCANO_SERVER_COMPONENT_HPP
#define VOLCANO_SERVER_COMPONENT_HPP

#include <Volcano/Server/Common.hpp>
#include <Volcano/Server/Object.hpp>

VOLCANO_SERVER_BEGIN

class Component: public Object {
public:
    Component(void);
    ~Component(void) override;
};

VOLCANO_SERVER_END

#endif // VOLCANO_SERVER_COMPONENT_HPP
