//
//
#include <memory>

#include <coreplugin/icore.h>
#include <coreplugin/messagebox.h>
#include <extensionsystem/pluginmanager.h>
#include <coreplugin/editormanager/editormanager.h>

#include <Volcano/Editor/NewProjectWizardFactory.hpp>
#include <Volcano/Editor/Plugin.hpp>

VOLCANO_EDITOR_BEGIN

Plugin::Plugin(void):
    m_gameWorldEditorFactory(nullptr)
{
}

Plugin::~Plugin(void)
{
    if (m_gameWorldEditorFactory != nullptr)
        delete m_gameWorldEditorFactory;
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

    auto gameWorldEditorFactory = std::make_unique<GameWorldEditorFactory>();
    if (!gameWorldEditorFactory)
        return false;

    m_gameWorldEditorFactory = gameWorldEditorFactory.release();

    return true;
}

VOLCANO_EDITOR_END
