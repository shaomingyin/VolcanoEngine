//
//
#include <Volcano/Editor/ProjectWizard.h>

VOLCANO_EDITOR_BEGIN

ProjectWizard::ProjectWizard(const Core::BaseFileWizardFactory* factory, const QVariantMap& extra_values, QWidget* parent)
    : Core::BaseFileWizard(factory, extra_values, parent) {
}

VOLCANO_EDITOR_END
