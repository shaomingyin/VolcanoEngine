//
//
#ifndef VOLCANO_GRAPHICS_WORLD_HPP
#define VOLCANO_GRAPHICS_WORLD_HPP

#include <QObject>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Camera.hpp>
#include <Volcano/Graphics/DrawingSet.hpp>

VOLCANO_GRAPHICS_BEGIN

class World: public QObject {
    Q_OBJECT

public:
    World(QObject *parent = nullptr);
    ~World(void) override;

public:
    virtual void buildDrawingSet(DrawingSet &out, const Camera &cam);
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_WORLD_HPP
