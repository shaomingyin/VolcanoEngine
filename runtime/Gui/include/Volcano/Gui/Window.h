//
//
#ifndef VOLCANO_GUI_WINDOW_H
#define VOLCANO_GUI_WINDOW_H

#include <string>

#include <SFML/Graphics/RenderTexture.hpp>

#include <Volcano/Gui/Common.h>
#include <Volcano/Gui/Context.h>
#include <Volcano/Gui/Widget.h>
#include <Volcano/Gui/Layout.h>
#include <Volcano/Gui/Object.h>

VOLCANO_GUI_BEGIN

class Window: public Object {
public:
    Window(Context& context, const std::string& title, int width, int height);
    virtual ~Window() = default;

public:
    const std::string& title() const noexcept {
        return title_;
    }

    void setTitle(const std::string& v) noexcept {
        title_ = v;
    }

    void setWidget(Widget* p) noexcept;
    void setLayout(Layout* p) noexcept;

private:
    Context& context_;
    std::string title_;
    sf::RenderTexture render_texture_;
    Widget* widget_;
    Layout* layout_;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_WINDOW_H
