//
//
#include <SFML/Graphics/RenderTexture.hpp>

#include <Volcano/Gui/Window.h>

VOLCANO_GUI_BEGIN

static sf::RenderTarget* createRenderTexture(int width, int height) {
    auto p = new sf::RenderTexture();
    p->create(width, height);
    return p;
}

Window::Window(Context& context, const std::string& title, int width, int height)
    : Object(float(width), float(height))
    , context_(context)
    , title_(title)
    , widget_(nullptr)
    , layout_(nullptr) {
    render_texture_.create(width, height);
}

void Window::setWidget(Widget* p) noexcept {
    widget_ = p;
    if (widget_ != nullptr) {
        layout_ = nullptr;
    }
}

void Window::setLayout(Layout* p) noexcept {
    layout_ = p;
    if (layout_ != nullptr) {
        widget_ = nullptr;
    }
}

VOLCANO_GUI_END
