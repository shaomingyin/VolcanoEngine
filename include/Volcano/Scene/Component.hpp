//
//
#ifndef VOLCANO_SCENE_COMPONENT_HPP
#define VOLCANO_SCENE_COMPONENT_HPP

#include <QObject>

#include <Volcano/Scene/Common.hpp>

VOLCANO_SCENE_BEGIN

class Component: public QObject
{
    Q_OBJECT

public:
    Component(QObject *parent = nullptr);
    ~Component(void) override;
};

VOLCANO_SCENE_END

#endif // VOLCANO_SCENE_COMPONENT_HPP
