//
//
#ifndef VOLCANO_EDITOR_PROPERTYVIEW_H
#define VOLCANO_EDITOR_PROPERTYVIEW_H

#include <coreplugin/navigationwidget.h>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class PropertyView: public Core::NavigationWidget {
    Q_OBJECT

public:
    PropertyView();
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PROPERTYVIEW_H
