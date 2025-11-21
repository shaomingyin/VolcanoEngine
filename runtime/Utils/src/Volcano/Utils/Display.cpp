//
//
#include <Volcano/Utils/Display.hpp>

VOLCANO_UTILS_BEGIN

Display::Display(Graphics::Context& context)
    : Graphics::Canvas(context)
    , gui_object_(nullptr) {
}

void Display::draw(sf::RenderTarget& target, sf::RenderStates states) {
    if (gui_object_ != nullptr) {
        target.draw(*gui_object_, states);
    }
}

VOLCANO_UTILS_END
