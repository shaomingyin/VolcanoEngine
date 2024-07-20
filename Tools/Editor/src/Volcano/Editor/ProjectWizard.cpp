//
//
#include <Volcano/Editor/ProjectWizard.hpp>

VOLCANO_EDITOR_BEGIN

ProjectWizard::ProjectWizard(const Core::BaseFileWizardFactory* factory, QWidget* parent,
                             const Core::WizardDialogParameters& parameters)
    : ProjectExplorer::BaseProjectWizardDialog(factory, parent, parameters) {
#if 0
        Core::WizardDialogParameters(Utils::FilePath::currentWorkingPath(),
            "VOLCANO", {}, Core::WizardDialogParameters::ForceCapitalLetterForFileName, {})) {
#endif
}

VOLCANO_EDITOR_END
