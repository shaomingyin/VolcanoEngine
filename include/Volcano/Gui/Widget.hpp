//
//
#ifndef VOLCANO_GUI_WIDGET_HPP
#define VOLCANO_GUI_WIDGET_HPP

#include <QObject>

#include <Volcano/Gui/Common.hpp>

VOLCANO_GUI_BEGIN

class VOLCANO_API Widget: public QObject
{
    Q_OBJECT

public:
    Widget(Widget *parent = nullptr);
    ~Widget(void) override;
};

VOLCANO_GUI_END

#endif // VOLCANO_GUI_WIDGET_HPP
