//
//
#ifndef VOLCANO_GUI_WIDGET_H
#define VOLCANO_GUI_WIDGET_H

#include <list>

#include <Volcano/Gui/Common.h>
#include <Volcano/Gui/Context.h>

VOLCANO_GUI_BEGIN

class Widget: public Context::Object {
public:
    using Children = std::list<Widget*>;

public:
    Widget(Widget& parent);
    Widget(Context& owner);
    virtual ~Widget();

public:
    const Children& children() const noexcept {
        return children_;
    }

    Widget* parent() const noexcept {
        return parent_;
    }

    void setParent(Widget* p);

    sf::Vector2i pos() const noexcept {
        return pos_;
    }

    void move(sf::Vector2i v) noexcept;
    
    sf::Vector2u size() const noexcept {
        return size_;
    }

    void resize(sf::Vector2i v) noexcept;

protected:
    void onPaint(sf::RenderTarget& target) noexcept override;
    void onResized(const sf::Event::SizeEvent& event) noexcept override;

private:
    Widget* parent_;
    Children children_;
    sf::Vector2i pos_;
    sf::Vector2u size_;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_WIDGET_H
