//
//
#ifndef VOLCANO_GUI_OBJECT_H
#define VOLCANO_GUI_OBJECT_H

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <Volcano/Gui/Common.h>

VOLCANO_GUI_BEGIN

class Object: public sf::Drawable {
public:
    Object(float width, float height);

public:
    bool isEnabled() const noexcept {
        return (flags_ & FlagEnabled);
    }

    void enable() noexcept {
        flags_ |= FlagEnabled;
    }

    void disable() noexcept {
        flags_ &= ~FlagEnabled;
    }

    bool isVisible() const noexcept {
        return (flags_ & FlagVisible);
    }

    void show() noexcept {
        flags_ |= FlagVisible;
    }

    void hide() noexcept {
        flags_ &= ~FlagVisible;
    }

    sf::Transform& transform() noexcept {
        return transform_;
    }

    const sf::Transform& transform() const noexcept {
        return transform_;
    }

    const sf::Color& backgroundColor() const noexcept {
        return background_.getFillColor();
    }

    void setBackgroundColor(const sf::Color& v) noexcept {
        background_.setFillColor(v);
    }

    const sf::Color& borderColor() const noexcept {
        return background_.getOutlineColor();
    }

    void setBorderColor(const sf::Color& v) noexcept {
        background_.setOutlineColor(v);
    }

    float borderWidth() const noexcept {
        return background_.getOutlineThickness();
    }

    void setBorderWidth(float v) noexcept {
        background_.setOutlineThickness(v);
    }

    const sf::Vector2f& size() const noexcept {
        return background_.getSize();
    }

    void resize(float width, float height) noexcept {
        background_.setSize({ width, height });
    }

    void resize(const sf::Vector2f& v) noexcept;
    virtual bool handleEvent(const sf::Event& event) noexcept;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    virtual void onPaint(sf::RenderTarget& target, sf::RenderStates states) const noexcept;
    virtual bool onResized(const sf::Event::SizeEvent& event) noexcept;

private:
    enum {
        FlagEnabled = 0x1,
        FlagVisible = 0x2
    };

private:
    int flags_;
    sf::Transform transform_;
    sf::RectangleShape background_;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_OBJECT_H
