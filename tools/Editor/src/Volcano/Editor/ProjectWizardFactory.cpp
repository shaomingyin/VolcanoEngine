//
//
#include <qtsupport/qtsupportconstants.h>

#include <Volcano/Editor/Project.h>
#include <Volcano/Editor/ProjectWizard.h>
#include <Volcano/Editor/ProjectWizardFactory.h>

VOLCANO_EDITOR_BEGIN

ProjectWizardFactory::ProjectWizardFactory() {
    setSupportedProjectTypes({Project::Id});
    setCategory("Others");
    setDescription("VolcanoEngine Editor.");
    setDisplayName("Volcano Project");
    setRequiredFeatures({
        QtSupport::Constants::FEATURE_QWIDGETS,
        QtSupport::Constants::FEATURE_DESKTOP
    });
}

Core::BaseFileWizard* ProjectWizardFactory::create(QWidget* parent, const Core::WizardDialogParameters& parameters) const {
    return new Volcano::Editor::ProjectWizard(this, parameters, parent);
}

Core::GeneratedFiles ProjectWizardFactory::generateFiles(const QWizard* wizard, QString* error_message) const {
    Core::GeneratedFiles gen_files;

    return gen_files;
}

bool ProjectWizardFactory::writeFiles(const Core::GeneratedFiles& files, QString* error_message) const {
    return true;
}

bool ProjectWizardFactory::postGenerateFiles(const QWizard* wizard, const Core::GeneratedFiles& l, QString* error_message) const {
    return true;
}

VOLCANO_EDITOR_END
