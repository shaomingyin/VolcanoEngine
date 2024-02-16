//
//
#ifndef VOLCANO_COMPONENT_HPP
#define VOLCANO_COMPONENT_HPP

#include <Volcano/Common.hpp>
#include <Volcano/Actor.hpp>

VOLCANO_BEGIN

class Component: public Actor {
    Q_OBJECT

public:
    Component(QObject* parent = nullptr);
    Component(Context& context, QObject* parent = nullptr);
};

VOLCANO_END

#endif // VOLCANO_COMPONENT_HPP
