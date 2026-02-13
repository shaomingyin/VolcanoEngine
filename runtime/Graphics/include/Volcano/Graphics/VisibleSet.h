//
//
#ifndef VOLCANO_GRAPHICS_VISIBLESET_H
#define VOLCANO_GRAPHICS_VISIBLESET_H

#include <vector>

#include <SFML/Graphics/Color.hpp>

#include <Volcano/Math.h>
#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class VisibleSet {
public:
    VisibleSet();

public:
    void clear() noexcept {
    }

    void merge(VisibleSet&& other) {
    }

private:
};

inline VisibleSet operator+(VisibleSet&& a, VisibleSet&& b) {
    VisibleSet ret;
    ret.merge(std::move(a));
    ret.merge(std::move(b));
    return ret;
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_VISIBLESET_H
