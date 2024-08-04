//
//
#include <QFile>

#include <projectexplorer/projectexplorer.h>
#include <projectexplorer/projectexplorericons.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <cmakeprojectmanager/cmakeprojectconstants.h>

#include <Volcano/Editor/BuildSystem.hpp>
#include <Volcano/Editor/Project.hpp>
#include <Volcano/Editor/ProjectWizard.hpp>
#include <Volcano/Editor/ProjectWizardFactory.hpp>

VOLCANO_EDITOR_BEGIN

ProjectWizardFactory::ProjectWizardFactory()
    : icon_(":/icons/logo.png") {
    setId("Z.VolcanoProjectFile");
    setSupportedProjectTypes({ Project::Id });
    setIcon(icon_);
    setDisplayName("Volcano Project");
    setDescription("XXX.");
    setCategory(ProjectExplorer::Constants::QT_PROJECT_WIZARD_CATEGORY);
    setDisplayCategory(ProjectExplorer::Constants::QT_PROJECT_WIZARD_CATEGORY_DISPLAY);
    setFlags(IWizardFactory::PlatformIndependent);
}

Core::BaseFileWizard *ProjectWizardFactory::create(QWidget *parent, const Core::WizardDialogParameters &parameters) const {
    return new class ProjectWizard(this, parent, parameters);
}

Core::GeneratedFiles ProjectWizardFactory::generateFiles(const QWizard *wizard, QString *error_message) const {
    const class ProjectWizard* project_wizard = qobject_cast<const class ProjectWizard*>(wizard);

    auto project_filepath = project_wizard->filePath().pathAppended(".volcano");
    Core::GeneratedFile project_file(project_filepath);
    project_file.setContents(QString::fromUtf8(BuildSystem::defaultProjectFileContents(project_wizard->projectName())));
    project_file.setAttributes(Core::GeneratedFile::OpenProjectAttribute);

    return { project_file };
}

bool ProjectWizardFactory::postGenerateFiles(const QWizard* wizard, const Core::GeneratedFiles& list, QString* error) const {
    for (const auto& file: list) {
        if (file.attributes() & Core::GeneratedFile::OpenProjectAttribute) {
            if (file.filePath().suffix() == "vpf") {
                auto result = ProjectExplorer::ProjectExplorerPlugin::openProject(file.filePath());
                if (!result) {
                    if (error) {
                        *error = result.errorMessage();
                    }
                    return false;
                }
            }
        }
    }
    return BaseFileWizardFactory::postGenerateFiles(wizard, list, error);
}

VOLCANO_EDITOR_END
