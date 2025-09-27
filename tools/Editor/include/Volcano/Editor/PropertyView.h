//
//
#ifndef VOLCANO_EDITOR_PROPERTYVIEW_H
#define VOLCANO_EDITOR_PROPERTYVIEW_H

#include <coreplugin/navigationwidget.h>

#include <Volcano/Editor/Common.h>
#include <Volcano/Editor/Context.h>

VOLCANO_EDITOR_BEGIN

class PropertyView: public Core::NavigationWidget {
    Q_OBJECT

public:
    PropertyView(Context& context);

private:
    Context& context_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PROPERTYVIEW_H
