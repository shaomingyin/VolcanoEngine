//
//
#include <Volcano/Editor/ProjectWizard.h>
#include <Volcano/Editor/ProjectWizardFactory.h>

VOLCANO_EDITOR_BEGIN

ProjectWizardFactory::ProjectWizardFactory() {
    setCategory("Utils");
    setDisplayCategory("Utils");
    setDisplayName("Volcano Project");
    setDescription("asdf");
}

Core::BaseFileWizard* ProjectWizardFactory::create(QWidget* parent, const Core::WizardDialogParameters& parameters) const {
    return new Editor::ProjectWizard(this, parameters.extraValues(), parent);
}

Core::GeneratedFiles ProjectWizardFactory::generateFiles(const QWizard* wizard, QString* error_message) const {
    return {};
}

bool ProjectWizardFactory::writeFiles(const Core::GeneratedFiles& files, QString* error_message) const {
    return false;
}

bool ProjectWizardFactory::postGenerateFiles(const QWizard* wizard, const Core::GeneratedFiles& gen_files, QString* error_message) const {
    return false;
}

VOLCANO_EDITOR_END
