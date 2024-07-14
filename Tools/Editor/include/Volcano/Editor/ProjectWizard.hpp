//
//
#ifndef VOLCANO_EDITOR_PROJECTWIZARD_HPP
#define VOLCANO_EDITOR_PROJECTWIZARD_HPP

#include <coreplugin/basefilewizard.h>
#include <projectexplorer/baseprojectwizarddialog.h>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class ProjectWizard: public ProjectExplorer::BaseProjectWizardDialog {
    Q_OBJECT

public:
    ProjectWizard(const Core::BaseFileWizardFactory *factory, QWidget *parent = nullptr);
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_PROJECTWIZARD_HPP
