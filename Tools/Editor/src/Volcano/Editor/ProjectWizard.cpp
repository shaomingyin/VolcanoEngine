//
//
#include <Volcano/Editor/ProjectWizard.hpp>

VOLCANO_EDITOR_BEGIN

ProjectWizard::ProjectWizard(const Core::BaseFileWizardFactory *factory, QWidget *parent)
    : Core::BaseFileWizard(factory, {}, parent) {
}

VOLCANO_EDITOR_END
