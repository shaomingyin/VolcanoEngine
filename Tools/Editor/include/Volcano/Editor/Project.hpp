//
//
#ifndef VOLCANO_EDITOR_PROJECT_HPP
#define VOLCANO_EDITOR_PROJECT_HPP

#include <QFuture>
#include <QVersionNumber>

#include <utils/filepath.h>
#include <projectexplorer/project.h>

#include <Volcano/Game/Context.hpp>
#include <Volcano/Game/World.hpp>
#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class Project: public ProjectExplorer::Project {
    Q_OBJECT

public:
    static constexpr const char Id[] = "Volcano.Editor.Project";
    static constexpr const char MimeType[] = VOLCANO_EDITOR_MIME_PROJECT_FILE;

public:
    Project(const Utils::FilePath &filename);

private:
    //Game::Context game_context_;
    Game::World game_world_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PROJECT_HPP
