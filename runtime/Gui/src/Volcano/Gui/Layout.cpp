//
//
#define LAY_IMPLEMENTATION
#include <layout.h>

#include <Volcano/Gui/Layout.hpp>

VOLCANO_GUI_BEGIN

Layout::Layout(Context& context, const sf::Vector2f& size, Object* parent)
    : Object(context, size, parent) {
}

VOLCANO_GUI_END
