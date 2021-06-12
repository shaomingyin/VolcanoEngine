//
//
#ifndef VOLCANO_GAME_COMPONENT_HPP
#define VOLCANO_GAME_COMPONENT_HPP

#include <QObject>

#include <Volcano/Game/Common.hpp>

VOLCANO_GAME_BEGIN

class Component: public QObject {
    Q_OBJECT

public:
    Component(QObject *parent = nullptr);
    ~Component(void) override;
};

VOLCANO_GAME_END

#endif // VOLCANO_GAME_COMPONENT_HPP
