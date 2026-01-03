//
//
#ifndef VOLCANO_GUI_WINDOW_H
#define VOLCANO_GUI_WINDOW_H

#include <layout.h>

#include <SFML/System/String.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Image.hpp>

#include <Volcano/Gui/Style.hpp>
#include <Volcano/Gui/Object.hpp>

VOLCANO_GUI_BEGIN

class Window: public Object {
public:
    enum {
        FlagTitle = 0x1,
        FlagBorder = 0x2,
        FlagResizable = 0x4
    };

    static constexpr int DefaultFlags = FlagTitle | FlagBorder | FlagResizable;

public:
    Window(Context& context, const sf::String& title, const sf::Vector2f& rect, int flags = DefaultFlags, Object* parent = nullptr);
    Window(Context& context, const sf::String& title, float width, float height, int flags = DefaultFlags, Object* parent = nullptr);
    virtual ~Window() = default;

public:
    const sf::String& getTitle() const noexcept {
        return title_;
    }

    void setTitle(const sf::String& title) noexcept {
        title_ = title;
    }

    void applyTitleStyle(const Style& style) {
        title_bar_.setFillColor(style.background_color);
        title_bar_.setOutlineColor(style.border_color);
        title_bar_.setOutlineThickness(style.border_size);
        title_text_.setFont(*context().getFont(style));
        title_text_.setCharacterSize(style.font_size);
    }

    void applyStyle(const Style& style) {
        background_.setFillColor(style.background_color);
        background_.setOutlineColor(style.border_color);
        background_.setOutlineThickness(style.border_size);
    }

    sf::FloatRect getClientRect() const noexcept {
        auto& size = getSize();
        auto title_height = title_bar_.getSize().y;
        return { { 0.0f, title_height }, { size.x, size.y - title_height } };
    }

protected:
    void onPaint(sf::RenderTarget& target, sf::RenderStates states) const override;
    void onMouseMoved(const sf::Event::MouseMoved& evt) override;
    void onMouseButtonPressed(const sf::Event::MouseButtonPressed& evt) override;
    void onMouseButtonReleased(const sf::Event::MouseButtonReleased& evt) override;
    virtual void onTitleChanged(const sf::String& title);

private:
    enum {
        FlagTitleBarHold = 0x1000,
    };

    int flags_;

    sf::String title_;
    sf::Image title_icon_;
    sf::RectangleShape title_bar_;
    FontCache::Pointer title_font_;
    sf::Text title_text_;
    lay_context title_lay_context_;
    lay_id title_text_lay_;
    sf::Vector2f title_bar_hold_pos_;
    sf::CircleShape title_bar_min_button_;
    sf::CircleShape title_bar_max_button_;
    sf::CircleShape title_bar_close_button_;

    sf::RectangleShape background_;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_WINDOW_H
