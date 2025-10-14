//
//
#ifndef VOLCANO_GUI_WINDOW_H
#define VOLCANO_GUI_WINDOW_H

#include <list>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderTarget.hpp>

#include <Volcano/Gui/Common.h>

VOLCANO_GUI_BEGIN

class Window {
public:
    class Object {
    public:
        Object(Window& owner);
        virtual ~Object() = default;
    
    public:
        Window& owner() noexcept {
            return owner_;
        }

        const Window& owner() const noexcept {
            return owner_;
        }

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

        void paint(sf::RenderTarget& target) noexcept;
        void event(const sf::Event& event) noexcept;

    protected:
        virtual void onPaint(sf::RenderTarget& target) noexcept = 0;
        virtual void onResized(const sf::Event::SizeEvent& event) noexcept = 0;

    private:
        enum {
            FlagEnabled = 0x1,
            FlagVisible = 0x2
        };

    private:
        Window& owner_;
        int flags_;
    };
    
    using Objects = std::list<Object*>;

public:
    Window(sf::RenderTarget& render_target);
    virtual ~Window() = default;

public:
    sf::Color backgroundColor() const noexcept {
        return background_.getFillColor();
    }

    void setBackgroundColor(sf::Color v) noexcept {
        background_.setFillColor(v);
    }

    sf::Color borderColor() const noexcept {
        return background_.getOutlineColor();
    }

    void setBorderColor(sf::Color v) noexcept {
        background_.setOutlineColor(v);
    }

    float borderWidth() const noexcept {
        return background_.getOutlineThickness();
    }

    void setBorderWidth(float v) noexcept {
        background_.setOutlineThickness(v);
    }

    const Objects& objects() const noexcept {
        return objects_;
    }

    void addObject(Object* p);
    void removeObject(Object* p);
    void moveToTop(Object* p);
    void paint() noexcept;
    void event(const sf::Event& event) noexcept;

private:
    sf::RenderTarget& render_target_;
    sf::RectangleShape background_;
    Objects objects_;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_WINDOW_H
