//
//
#ifndef VOLCANO_EDITOR_BUILDSYSTEM_HPP
#define VOLCANO_EDITOR_BUILDSYSTEM_HPP

#include <QString>

#include <utils/id.h>
#include <projectexplorer/target.h>
#include <projectexplorer/buildsystem.h>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class BuildSystem: public ProjectExplorer::BuildSystem {
    Q_OBJECT

public:
    static constexpr char ID[] = "VolcanoProjectManager.BuildSystem";

public:
    BuildSystem(ProjectExplorer::Target *target);
    ~BuildSystem(void) override;

public:
    QString name(void) const override;
    void triggerParsing(void) override;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_BUILDSYSTEM_HPP
