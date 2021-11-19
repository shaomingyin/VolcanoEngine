//
//
#ifndef VOLCANO_GRAPHICS_VIEWABLE_HPP
#define VOLCANO_GRAPHICS_VIEWABLE_HPP

#include <QObject>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/VisibleSet.hpp>
#include <Volcano/Graphics/Camera.hpp>

VOLCANO_GRAPHICS_BEGIN

class Viewable: public QObject {
    Q_OBJECT

public:
    Viewable(QObject *parent = nullptr);
    ~Viewable(void) override;

public:
    virtual void buildVisibleSet(VisibleSet &out, Camera &cam) = 0;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_VIEWABLE_HPP
