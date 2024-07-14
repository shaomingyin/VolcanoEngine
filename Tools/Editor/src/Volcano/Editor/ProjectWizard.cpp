//
//
#include <Volcano/Editor/ProjectWizard.hpp>

VOLCANO_EDITOR_BEGIN

ProjectWizard::ProjectWizard(const Core::BaseFileWizardFactory *factory, QWidget *parent)
    : ProjectExplorer::BaseProjectWizardDialog(
        factory, parent,
        Core::WizardDialogParameters(Utils::FilePath::currentWorkingPath(),
            "VOLCANO", {}, Core::WizardDialogParameters::ForceCapitalLetterForFileName, {})) {
}

VOLCANO_EDITOR_END
