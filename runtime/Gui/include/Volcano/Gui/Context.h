//
//
#ifndef VOLCANO_GUI_CONTEXT_H
#define VOLCANO_GUI_CONTEXT_H

#include <list>

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

#include <Volcano/Gui/Common.h>
#include <Volcano/Gui/Object.h>

VOLCANO_GUI_BEGIN

using Objects = std::list<Object*>;

class Context {
public:
    Context(sf::RenderTarget& render_target);
    virtual ~Context() = default;

public:
    sf::RectangleShape& background() noexcept {
        return background_;
    }

    const sf::RectangleShape& background() const noexcept {
        return background_;
    }

    const Objects& objects() const noexcept {
        return objects_;
    }

    Object* currentObject() const noexcept {
        return current_object_;
    }

    void makeCurrentObject(Object* p);
    void makeTopObject(Object* p);
    void addObject(Object* p);
    void removeObject(Object* p);
    void render() noexcept;
    void handleEvent(const sf::Event& event) noexcept;

protected:
    virtual void onResized(const sf::Event::SizeEvent& event) noexcept;

private:
    void drawDecoration();

private:
    sf::RenderTarget& render_target_;
    sf::RectangleShape background_;
    Objects objects_;
    Object* current_object_;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_CONTEXT_H
