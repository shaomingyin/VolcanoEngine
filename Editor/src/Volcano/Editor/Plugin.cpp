//
//
#include <coreplugin/icore.h>
#include <extensionsystem/pluginmanager.h>

#include <Volcano/Editor/NewProjectWizardFactory.hpp>
#include <Volcano/Editor/Plugin.hpp>

VOLCANO_EDITOR_BEGIN

Plugin::Plugin(void):
    m_newProjectWizardFactory(nullptr)
{
}

Plugin::~Plugin(void)
{
    if (m_newProjectWizardFactory != nullptr)
        delete m_newProjectWizardFactory;
}

Plugin::ShutdownFlag Plugin::aboutToShutdown(void)
{
    return SynchronousShutdown;
}

void Plugin::extensionsInitialized(void)
{
}

bool Plugin::initialize(const QStringList &arguments, QString *errorString)
{
    Core::IWizardFactory::registerFactoryCreator([] {
        return QList<Core::IWizardFactory *> { new NewProjectWizardFactory() };
    });

    return true;
}

VOLCANO_EDITOR_END
