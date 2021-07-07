//
//
#ifndef VOLCANO_GRAPHICS_RENDERABLE_HPP
#define VOLCANO_GRAPHICS_RENDERABLE_HPP

#include <QVector3D>
#include <QQuaternion>

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class Renderable {
public:
    Renderable(void);
    virtual ~Renderable(void);

public:
    const QVector3D &position(void) const;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERABLE_HPP
