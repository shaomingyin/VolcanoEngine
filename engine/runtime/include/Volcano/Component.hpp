//
//
#ifndef VOLCANO_COMPONENT_HPP
#define VOLCANO_COMPONENT_HPP

#include <QObject>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class Component: public QObject {
    Q_OBJECT

public:
    Component(QObject *parent = nullptr);
    ~Component(void) override;
};

VOLCANO_END

#endif // VOLCANO_COMPONENT_HPP
