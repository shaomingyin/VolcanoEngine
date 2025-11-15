//
//
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics/Image.hpp>

#include <Volcano/Gui/Window.hpp>

VOLCANO_GUI_BEGIN

Window::Window(Context& context, const sf::String& title, const sf::Vector2f& size, int flags, Object* parent)
    : Object(context, size, parent)
    , flags_(flags & (FlagTitle | FlagBorder))
    , title_(title) {
    if (flags_ & FlagTitle) {
        auto window_title_height = context.getWindowTitleHeight();
        auto& title_style = context.getWindowTitleStyle();
        title_font_ = context.getFont(title_style);
        title_text_.setString(title_);
        title_text_.setFont(*title_font_);
        title_text_.setCharacterSize(title_style.font_size);
        title_text_.setFillColor(title_style.font_color);
        title_bar_.setFillColor(title_style.background_color);
        title_bar_.setOutlineColor(title_style.border_color);
        title_bar_.setOutlineThickness(title_style.border_size);
        title_bar_.setSize({ size.x, window_title_height });
    }

    auto& window_style = context.getDefaultStyle();
    background_.setFillColor(window_style.background_color);
    background_.setSize(size);

    if (flags_ & FlagBorder) {
        background_.setOutlineColor(window_style.border_color);
        background_.setOutlineThickness(window_style.border_size);
    } else {
        background_.setOutlineColor(sf::Color::Transparent);
        background_.setOutlineThickness(0.0f);
    }
}

Window::Window(Context& context, const sf::String& title, float width, float height, int flags, Object* parent)
    : Window(context, title, { width, height }, flags, parent) {
}

void Window::onPaint(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(background_, states);

    if (flags_ & FlagTitle) {
        target.draw(title_bar_, states);
        target.draw(title_text_, states);
    }
}

void Window::onMouseMoved(const sf::Event::MouseMoveEvent& evt) {
    if (flags_ & FlagTitleBarHold) {
        auto gpos = toGlobal(evt.x, evt.y);
        move(float(gpos.x) - title_bar_hold_pos_.x, float(gpos.y) - title_bar_hold_pos_.y);
        title_bar_hold_pos_ = gpos;
    }

    if (flags_ & FlagResizable) {
        // TODO resize
    }
}

void Window::onMouseButtonPressed(const sf::Event::MouseButtonEvent& evt) {
    auto size = title_bar_.getSize();
    if (evt.x >= 0 && evt.x <= size.x && evt.y >= 0 && evt.y <= size.y) {
        flags_ |= FlagTitleBarHold;
        grabMouse();
        title_bar_hold_pos_ = toGlobal(evt.x, evt.y);
    }
}

void Window::onMouseButtonReleased(const sf::Event::MouseButtonEvent& evt) {
    flags_ &= ~FlagTitleBarHold;
    releaseMouse();
}

void Window::onTitleChanged(const sf::String& title) {
}

VOLCANO_GUI_END
