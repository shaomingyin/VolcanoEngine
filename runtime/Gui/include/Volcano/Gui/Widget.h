//
//
#ifndef VOLCANO_GUI_WIDGET_H
#define VOLCANO_GUI_WIDGET_H

#include <list>

#include <Volcano/Gui/Common.h>
#include <Volcano/Gui/Context.h>
#include <Volcano/Gui/Object.h>

VOLCANO_GUI_BEGIN

class Widget: public Object {
public:
    using Children = std::list<Widget*>;

public:
    Widget(float width, float height, Widget* parent = nullptr);
    virtual ~Widget();

public:
    const Children& children() const noexcept {
        return children_;
    }

    Widget* parent() const noexcept {
        return parent_;
    }

    void setParent(Widget* p);
    bool handleEvent(const sf::Event& event) noexcept override;

protected:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

private:
    Widget* parent_;
    Children children_;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_WIDGET_H
