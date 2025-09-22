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

Core::BaseFileWizard* ProjectWizardFactory::create(const Core::WizardDialogParameters& parameters) const {
    return new Editor::ProjectWizard(this, parameters.extraValues());
}

Core::GeneratedFiles ProjectWizardFactory::generateFiles(const QWizard* wizard, QString* error_message) const {
    return {};
}

Utils::Result<> ProjectWizardFactory::writeFiles(const Core::GeneratedFiles& files) const {
    return Utils::ResultOk;
}

Utils::Result<> ProjectWizardFactory::postGenerateFiles(const QWizard* wizard, const Core::GeneratedFiles& gen_files) const {
    return Utils::ResultOk;
}

VOLCANO_EDITOR_END
