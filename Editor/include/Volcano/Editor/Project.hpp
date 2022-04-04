//
//
#ifndef VOLCANO_EDITOR_PROJECT_HPP
#define VOLCANO_EDITOR_PROJECT_HPP

#include <memory>
#include <type_traits>

#include <utils/filepath.h>
#include <projectexplorer/project.h>

#include <Volcano/Game/Context.hpp>
#include <Volcano/Game/World.hpp>

#include <Volcano/Editor/Common.hpp>
#include <Volcano/Editor/GameContext.hpp>

VOLCANO_EDITOR_BEGIN

class Project: public ProjectExplorer::Project {
    Q_OBJECT

public:
    static constexpr char ID[] = "VolcanoProjectManager.Project";

public:
    Project(const Utils::FilePath &fileName);
    ~Project(void) override;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PROJECT_HPP
