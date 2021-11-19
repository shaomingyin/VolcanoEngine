//
//
#ifndef VOLCANO_GRAPHICS_VISIBLESET_HPP
#define VOLCANO_GRAPHICS_VISIBLESET_HPP

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class VisibleSet {
public:
    VisibleSet(void);
    virtual ~VisibleSet(void);

public:
    virtual void reset(void);
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_VISIBLESET_HPP
