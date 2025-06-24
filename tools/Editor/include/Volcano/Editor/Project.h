//
//
#ifndef VOLCANO_EDITOR_PROJECT_H
#define VOLCANO_EDITOR_PROJECT_H

#include <QString>

#include <utils/id.h>
#include <projectexplorer/project.h>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class Project: public ProjectExplorer::Project {
    Q_OBJECT

public:
    static const Utils::Id Id;
    static const QString MimeType;

public:
    explicit Project(const Utils::FilePath& file_name);

protected:
    void configureAsExampleProject(ProjectExplorer::Kit* kit) override;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PROJECT_H
