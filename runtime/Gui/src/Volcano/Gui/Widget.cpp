//
//
#include <Volcano/Gui/Widget.hpp>

VOLCANO_GUI_BEGIN

Widget::Widget(Context& context, const sf::Vector2f& size, Object* parent)
    : Object(context, size, parent)
    , flags_(0) {
}

Widget::Widget(Context& context, float width, float height, Object* parent)
    : Widget(context, { width, height }, parent) {
}

void Widget::onPaint(sf::RenderTarget& target, sf::RenderStates states) const noexcept {
}

void Widget::onMouseMoved(const sf::Event::MouseMoved& evt) noexcept {
}

void Widget::onMouseLeft() noexcept {
}

void Widget::onMouseButtonPressed(const sf::Event::MouseButtonPressed& evt) noexcept {
}

void Widget::onMouseButtonReleased(const sf::Event::MouseButtonReleased& evt) noexcept {
}

VOLCANO_GUI_END
