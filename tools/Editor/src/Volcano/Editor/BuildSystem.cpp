//
//
#include <QDir>
#include <QFileInfo>
#include <QPromise>
#include <QtConcurrent>

#include <coreplugin/progressmanager/progressmanager.h>
#include <projectexplorer/project.h>
#include <projectexplorer/projectnodes.h>

#include <Volcano/Editor/BuildSystem.h>

VOLCANO_EDITOR_BEGIN

BuildSystem::BuildSystem(ProjectExplorer::Target* target)
    : ProjectExplorer::BuildSystem(target) {
    connect(target->project(), &ProjectExplorer::Project::projectFileIsDirty, this, [this](const Utils::FilePath &p) {
    });
    connect(target, &ProjectExplorer::Target::activeBuildConfigurationChanged, this, [this, target] {
    });
    connect(project(), &ProjectExplorer::Project::activeTargetChanged, this, [this, target] {
    });
}

BuildSystem::BuildSystem(ProjectExplorer::BuildConfiguration* build_configuration)
    : ProjectExplorer::BuildSystem(build_configuration) {
}

QString BuildSystem::name() const {
    return "Volcano";
}

void BuildSystem::triggerParsing() {
#if 0
    parsing_future_ = QtConcurrent::run([this](QPromise<std::unique_ptr<ProjectExplorer::ProjectNode>>& promise) {
        parse(promise);
    }).then(this, [this](std::unique_ptr<ProjectExplorer::ProjectNode> node) {
        project()->setRootProjectNode(std::move(node));
    }).onCanceled(this, [this] {
    }).onFailed(this, [this] {
    });
    Core::ProgressManager::addTask(parsing_future_, "Parsing Volcano project file....", "");
#endif
}

void BuildSystem::parse(QPromise<void>& promise) {
    auto project_filepath = project()->projectFilePath();

    auto node = std::make_unique<ProjectExplorer::ProjectNode>("asdf");

    //promise.setProgressRange()

    //promise.setProgressValueAndText()

    //promise.emplaceResult(std::move(node));
}

VOLCANO_EDITOR_END
