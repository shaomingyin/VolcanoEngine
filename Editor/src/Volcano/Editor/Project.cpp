//
//
#include <memory>

#include <coreplugin/icontext.h>
#include <projectexplorer/projectexplorerconstants.h>

#include <Volcano/Editor/BuildSystem.hpp>
#include <Volcano/Editor/Project.hpp>

VOLCANO_EDITOR_BEGIN

Project::Project(const Utils::FilePath &fileName):
    ProjectExplorer::Project(VOLCANO_EDITOR_WORLD_MIMETYPE, fileName)
{
    qDebug() << __FUNCTION__;

    setId(ID);
    setProjectLanguages(Core::Context(ProjectExplorer::Constants::QMLJS_LANGUAGE_ID));
    setDisplayName(projectDirectory().fileName());
    setCanBuildProducts();
    setHasMakeInstallEquivalent(true);

    setBuildSystemCreator([] (ProjectExplorer::Target *target) {
        qDebug() << "__FUNCTION__ CREATE BUILD SYSTEM";
        return new BuildSystem(target);
    });
}

Project::~Project(void)
{
}

VOLCANO_EDITOR_END
