//
//
#ifndef VOLCANO_GUI_OBJECT_H
#define VOLCANO_GUI_OBJECT_H

#include <list>

#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <Volcano/Gui/Common.h>
#include <Volcano/Gui/Context.h>

VOLCANO_GUI_BEGIN

class Object: public sf::Drawable, public sf::Transformable {
public:
    using ObjectList = std::list<Object*>;

public:
    Object(Context& context, float width, float height, Object* parent = nullptr);
    Object(Context& context, const sf::Vector2f& size, Object* parent = nullptr);
    virtual ~Object();

public:
    Context& context() noexcept {
        return context_;
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

    Object* getParent() noexcept {
        return parent_;
    }

    void setParent(Object* parent);

    const ObjectList& getChildren() const noexcept {
        return children_;
    }

    const sf::Vector2f& getSize() const noexcept {
        return size_;
    }

    void setSize(float width, float height) noexcept {
        size_.x = width;
        size_.y = height;
    }

    void setSize(const sf::Vector2f& v) noexcept {
        size_ = v;
    }

    sf::FloatRect getRect() const noexcept {
        auto& position = getPosition();
        return { position.x, position.y, size_.x, size_.y };
    }

    void setRect(const sf::FloatRect& v) noexcept {
        setPosition(v.getPosition());
        setSize(v.getSize());
    }

    sf::Vector2f toLocal(float x, float y) const noexcept;
    sf::Vector2f toLocal(const sf::Vector2f& v) const noexcept;
    sf::FloatRect toLocal(const sf::FloatRect& v) const noexcept;
    sf::FloatRect toLocal(float left, float top, float width, float height) const noexcept;
    sf::Vector2f toGlobal(float x, float y) const noexcept;
    sf::Vector2f toGlobal(const sf::Vector2f& v) const noexcept;
    sf::FloatRect toGlobal(const sf::FloatRect& v) const noexcept;
    sf::FloatRect toGlobal(float left, float top, float width, float height) const noexcept;

    void toTop();
    bool event(const sf::Event& event);

protected:
    bool isMouseInside() const noexcept {
        return (flags_ & FlagMouseInside);
    }

    bool isMouseGrabbed() const noexcept {
        return (flags_ & FlagMouseGrabbed);
    }

    void grabMouse() noexcept {
        flags_ |= FlagMouseGrabbed;
    }

    void releaseMouse() noexcept {
        flags_ &= ~FlagMouseGrabbed;
    }

    virtual void onChildAdded(Object* child);
    virtual void onChildRemoved(Object* child);
    virtual void onPaint(sf::RenderTarget& target, sf::RenderStates state) const = 0;
    virtual bool onEvent(const sf::Event& evt);
    virtual void onMouseMoved(const sf::Event::MouseMoveEvent& evt);
    virtual void onMouseEntered();
    virtual void onMouseLeft();
    virtual void onMouseButtonPressed(const sf::Event::MouseButtonEvent& evt);
    virtual void onMouseButtonReleased(const sf::Event::MouseButtonEvent& evt);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    bool handleMouseMoved(const sf::Event::MouseMoveEvent& evt);
    bool handleMouseEntered();
    bool handleMouseLeft();
    bool handleMouseButtonPressed(const sf::Event::MouseButtonEvent& evt);
    bool handleMouseButtonReleased(const sf::Event::MouseButtonEvent& evt);

private:
    enum {
        FlagEnabled = 0x1,
        FlagVisible = 0x2,
        FlagMouseInside = 0x4,
        FlagMouseGrabbed = 0x8
    };

private:
    Context& context_;
    int flags_;
    Object* parent_;
    ObjectList children_;
    sf::Vector2f size_;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_OBJECT_H
