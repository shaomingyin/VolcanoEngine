//
//
#ifndef VOLCANO_EDITOR_PROJECTWIZARD_H
#define VOLCANO_EDITOR_PROJECTWIZARD_H

#include <coreplugin/basefilewizardfactory.h>

#include <projectexplorer/baseprojectwizarddialog.h>

#include <Volcano/Editor/Common.h>

VOLCANO_EDITOR_BEGIN

class ProjectWizard: public ProjectExplorer::BaseProjectWizardDialog {
    Q_OBJECT

public:
    ProjectWizard(const Core::BaseFileWizardFactory* factory, const Core::WizardDialogParameters &parameters, QWidget *parent);
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PROJECTWIZARD_H
