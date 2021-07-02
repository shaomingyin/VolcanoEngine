//
//
#ifndef VOLCANO_GRAPHICS_VISIBLESET_HPP
#define VOLCANO_GRAPHICS_VISIBLESET_HPP

#include <QRectF>
#include <QColor>
#include <QVector3D>

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class VisibleSet {
public:
    VisibleSet(void);
    virtual ~VisibleSet(void);

public:
    void reset(void);

private:
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_VISIBLESET_HPP
