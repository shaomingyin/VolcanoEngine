//
//
#include <Volcano/Editor/ProjectWizard.h>

VOLCANO_EDITOR_BEGIN

ProjectWizard::ProjectWizard(const Core::BaseFileWizardFactory* factory, const Core::WizardDialogParameters &parameters, QWidget *parent)
    : ProjectExplorer::BaseProjectWizardDialog(factory, parent, parameters) {

}

VOLCANO_EDITOR_END
