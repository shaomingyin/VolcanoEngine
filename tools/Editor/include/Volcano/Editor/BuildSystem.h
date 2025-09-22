//
//
#ifndef VOLCANO_EDITOR_BUILDSYSTEM_H
#define VOLCANO_EDITOR_BUILDSYSTEM_H

#include <projectexplorer/buildsystem.h>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class BuildSystem: public ProjectExplorer::BuildSystem {
    Q_OBJECT

public:
    explicit BuildSystem(ProjectExplorer::BuildConfiguration *build_configuration);
    ~BuildSystem() override;

public:
    void triggerParsing() override;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_BUILDSYSTEM_H
