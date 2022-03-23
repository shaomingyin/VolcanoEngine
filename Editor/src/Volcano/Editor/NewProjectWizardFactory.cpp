//
//
#include <memory>

#include <QIcon>

#include <projectexplorer/projectexplorerconstants.h>

#include <Volcano/Editor/NewProjectWizard.hpp>
#include <Volcano/Editor/NewProjectWizardFactory.hpp>

VOLCANO_EDITOR_BEGIN

NewProjectWizardFactory::NewProjectWizardFactory(void)
{
    setSupportedProjectTypes({ "vpf" });
    setIcon(QIcon(":/Images/Logo.png"));
    setDisplayName(tr("Volcano Engine Project"));
    //setId("Z.Makefile");
    setDescription(tr("Create new project for VolcanoEngine."));
    setCategory("Volcano Engine");
    setDisplayCategory("Volcano Engine");
    setFlags(Core::IWizardFactory::PlatformIndependent);
}

NewProjectWizardFactory::~NewProjectWizardFactory(void)
{
}

Utils::Wizard *NewProjectWizardFactory::runWizardImpl(const Utils::FilePath &path,
    QWidget *parent, Utils::Id platform, const QVariantMap &variables, bool showWizard)
{
    auto newProjectWizard = std::make_unique<NewProjectWizard>(path, parent);
    if (!newProjectWizard)
        return nullptr;

    newProjectWizard->setVisible(showWizard);

    return newProjectWizard.release();
}

VOLCANO_EDITOR_END
