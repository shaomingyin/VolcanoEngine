//
//
#ifndef VOLCANO_EDITOR_PROJECTVIEW_H
#define VOLCANO_EDITOR_PROJECTVIEW_H

#include <coreplugin/navigationwidget.h>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class ProjectView: public Core::NavigationWidget {
    Q_OBJECT

public:
    ProjectView(QObject* parent = nullptr);
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PROJECTVIEW_H
