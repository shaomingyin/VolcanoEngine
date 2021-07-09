//
//
#include <Volcano/Editor/NewProjectWizard.hpp>

VOLCANO_EDITOR_BEGIN

NewProjectWizard::NewProjectWizard(QWidget *parent):
    QWizard(parent)
{
    setWindowTitle("New Project");

    addPage(&m_newProjectInfoPage);
}

NewProjectWizard::~NewProjectWizard(void)
{
}

VOLCANO_EDITOR_END
