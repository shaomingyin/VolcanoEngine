//
//
#ifndef VOLCANO_GUI_WIDGET_H
#define VOLCANO_GUI_WIDGET_H

#include <layout.h>

#include <SFML/System/String.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <Volcano/Gui/Object.h>

VOLCANO_GUI_BEGIN

class Widget : public Object {
public:
    enum {
    };

public:
    Widget(Context& context, const sf::Vector2f& size, Object* parent = nullptr);
    Widget(Context& context, float width, float height, Object* parent = nullptr);
    virtual ~Widget() = default;

public:

protected:
    void onPaint(sf::RenderTarget& target, sf::RenderStates states) const noexcept override;
    void onMouseMoved(const sf::Event::MouseMoveEvent& evt) noexcept override;
    void onMouseLeft() noexcept override;
    void onMouseButtonPressed(const sf::Event::MouseButtonEvent& evt) noexcept override;
    void onMouseButtonReleased(const sf::Event::MouseButtonEvent& evt) noexcept override;

private:
    int flags_;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_WIDGET_H
