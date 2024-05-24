//
//
#include <projectexplorer/projectexplorericons.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <cmakeprojectmanager/cmakeprojectconstants.h>

#include <Volcano/Editor/ProjectWizard.hpp>
#include <Volcano/Editor/ProjectWizardFactory.hpp>

VOLCANO_EDITOR_BEGIN

ProjectWizardFactory::ProjectWizardFactory() {
    setSupportedProjectTypes({CMakeProjectManager::Constants::CMAKE_PROJECT_ID});
    setIcon(ProjectExplorer::Icons::WINDOW.icon());
    setDisplayName("Volcano Project");
    setId("V.VolcanoProjectFile");
    setDescription("XXX.");
    setCategory(ProjectExplorer::Constants::QT_PROJECT_WIZARD_CATEGORY);
    setDisplayCategory(ProjectExplorer::Constants::QT_PROJECT_WIZARD_CATEGORY_DISPLAY);
    setFlags(IWizardFactory::PlatformIndependent);
}

Core::BaseFileWizard *ProjectWizardFactory::create(QWidget *parent, const Core::WizardDialogParameters &parameters) const {
    return new class ProjectWizard(this, parent);
}

Core::GeneratedFiles ProjectWizardFactory::generateFiles(const QWizard *wizard, QString *error_message) const {
    const class ProjectWizard* project_wizard = qobject_cast<const class ProjectWizard*>(wizard);
    // TODO
    return {};
}

VOLCANO_EDITOR_END
