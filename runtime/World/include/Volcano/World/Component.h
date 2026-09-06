//
//
#ifndef VOLCANO_WORLD_COMPONENT_H
#define VOLCANO_WORLD_COMPONENT_H

#include <QObject>

#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Entity;

class Component: public QObject {
    Q_OBJECT

public:
    Component(QObject* parent = nullptr);
    ~Component() override;

public:
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_COMPONENT_H
