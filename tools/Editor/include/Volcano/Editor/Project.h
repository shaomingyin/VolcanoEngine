//
//
#ifndef VOLCANO_EDITOR_PROJECT_H
#define VOLCANO_EDITOR_PROJECT_H

#include <QString>

#include <projectexplorer/project.h>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class Project: public ProjectExplorer::Project {
    Q_OBJECT

public:
    static const QString MimeType;

public:
    Project(const Utils::FilePath &fileName);
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PROJECT_H
