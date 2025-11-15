//
//
#ifndef VOLCANO_GUI_LAYOUT_H
#define VOLCANO_GUI_LAYOUT_H

#include <Volcano/Gui/Common.hpp>
#include <Volcano/Gui/Object.hpp>

VOLCANO_GUI_BEGIN

class Layout: public Object {
public:
    Layout(Context& context, const sf::Vector2f& size, Object* parent = nullptr);
    Layout(Context& context, float width, float height, Object* parent = nullptr);

public:
    
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_LAYOUT_H
