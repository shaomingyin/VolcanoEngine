//
//
#ifndef VOLCANO_COMPONENT_HPP
#define VOLCANO_COMPONENT_HPP

#include <Volcano/Common.hpp>
#include <Volcano/Object.hpp>

VOLCANO_BEGIN

class Component: public Object
{
    Q_OBJECT

public:
    Component(QObject *parent = nullptr);
    ~Component(void) override;
};

VOLCANO_END

#endif // VOLCANO_COMPONENT_HPP
