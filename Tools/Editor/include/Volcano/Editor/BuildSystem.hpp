//
//
#ifndef VOLCANO_EDITOR_BUILDSYSTEM_HPP
#define VOLCANO_EDITOR_BUILDSYSTEM_HPP

#include <projectexplorer/buildsystem.h>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class BuildSystem: public ProjectExplorer::BuildSystem {
    Q_OBJECT

public:
    BuildSystem(ProjectExplorer::Target* target);

public:
    void triggerParsing() override;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_BUILDSYSTEM_HPP
