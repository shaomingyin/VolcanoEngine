//
//
#ifndef VOLCANO_WORLD_COMPONENT_H
#define VOLCANO_WORLD_COMPONENT_H

#include <Volcano/World/Common.h>
#include <Volcano/World/Object.h>

VOLCANO_WORLD_BEGIN

class Component: public Object {
    Q_OBJECT

public:
    Component(QObject* parent = nullptr);
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_COMPONENT_H
