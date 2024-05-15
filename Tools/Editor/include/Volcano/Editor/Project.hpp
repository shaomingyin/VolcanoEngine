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
    Project(const Utils::FilePath &filename);

public:
    const QVersionNumber& version() const {
        return version_;
    }

    const QVersionNumber& engineVersion() const {
        return engine_version_;
    }

protected:
    void startLoading();

private:
    void load();
    bool loadGame(const QJsonObject& obj);
    bool loadWorld(const QJsonObject& obj);
    bool loadScene(const QJsonObject& obj);
    void loadExtraProjects(const QJsonObject& obj);
    static void parseVersion(const QJsonObject& obj, const QString& key, QVersionNumber& out, const QVersionNumber& def);

private:
    QVersionNumber version_;
    QVersionNumber engine_version_;

private:
    Game::Context game_context_;
    Game::World game_world_;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PROJECT_HPP
