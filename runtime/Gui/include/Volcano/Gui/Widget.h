//
//
#ifndef VOLCANO_GUI_WIDGET_H
#define VOLCANO_GUI_WIDGET_H

#include <Volcano/Gui/Common.h>
#include <Volcano/Gui/Context.h>

VOLCANO_GUI_BEGIN

class Widget: public Context::WidgetBase {
public:
    Widget(Widget& parent);
    Widget(Context& context);
    ~Widget() override ;

public:
    Context& context() {
        return context_;
    }

    const Context& context() const {
        return context_;
    }

    void handleEvent(const SDL_Event& evt);

private:
    Context& context_;
    Widget* parent_;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_WIDGET_H
