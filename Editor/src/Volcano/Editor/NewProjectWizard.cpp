//
//
#include <Volcano/Editor/NewProjectWizardPageIntro.hpp>
#include <Volcano/Editor/NewProjectWizardPageBasic.hpp>
#include <Volcano/Editor/NewProjectWizardPageSummary.hpp>
#include <Volcano/Editor/NewProjectWizard.hpp>

VOLCANO_EDITOR_BEGIN

NewProjectWizard::NewProjectWizard(const Utils::FilePath &path, QWidget *parent):
    Utils::Wizard(parent),
    m_path(path)
{
    registerFieldName("ProjectName");
    registerFieldName("ProjectDirectory");

    setPage(PageIntro, new NewProjectWizardPageIntro);
    setPage(PageBasic, new NewProjectWizardPageBasic);
    setPage(PageSummary, new NewProjectWizardPageSummary);
}

NewProjectWizard::~NewProjectWizard(void)
{
}

VOLCANO_EDITOR_END
