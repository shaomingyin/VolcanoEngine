//
//
#ifndef VOLCANO_GRAPHICS_DRAWINGSET_HPP
#define VOLCANO_GRAPHICS_DRAWINGSET_HPP

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class DrawingSet {
public:
    DrawingSet(void);
    virtual ~DrawingSet(void);

public:
    void clear(void);
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_DRAWINGSET_HPP
