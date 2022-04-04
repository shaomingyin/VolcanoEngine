//
//
#include <memory>

#include <utils/mimetypes/mimedatabase.h>
#include <extensionsystem/pluginmanager.h>
#include <coreplugin/icore.h>
#include <coreplugin/messagebox.h>
#include <coreplugin/editormanager/editormanager.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/kitmanager.h>
#include <projectexplorer/projectmanager.h>
#include <projectexplorer/toolchainmanager.h>

#include <Volcano/Editor/ToolChain.hpp>
#include <Volcano/Editor/Project.hpp>
#include <Volcano/Editor/Plugin.hpp>

VOLCANO_EDITOR_BEGIN

Plugin::Plugin(void):
    m_buildConfigurationFactctory(nullptr),
    m_gameWorldEditorFactory(nullptr),
    m_gameObjectPropertyViewFactory(nullptr)
{
}

Plugin::~Plugin(void)
{
    if (m_buildConfigurationFactctory != nullptr)
        delete m_buildConfigurationFactctory;
    if (m_gameWorldEditorFactory != nullptr)
        delete m_gameWorldEditorFactory;
    if (m_gameObjectPropertyViewFactory != nullptr)
        delete m_gameObjectPropertyViewFactory;
}

Plugin::ShutdownFlag Plugin::aboutToShutdown(void)
{
    return SynchronousShutdown;
}

void Plugin::extensionsInitialized(void)
{
    ProjectExplorer::ToolChainManager::registerLanguage(ProjectExplorer::Constants::QMLJS_LANGUAGE_ID, "QmlJs");

    auto tc = std::make_unique<ToolChain>();
    tc->setLanguage(ProjectExplorer::Constants::QMLJS_LANGUAGE_ID);
    tc->setDetection(ProjectExplorer::ToolChain::AutoDetection);
    tc->setDisplayName("VolcanoSdk");
    tc->setDetectionSource("VolcanoDetectionSource");
    ProjectExplorer::ToolChainManager::registerToolChain(tc.release());

}

bool Plugin::initialize(const QStringList &arguments, QString *errorString)
{
    m_buildConfigurationFactctory = new BuildConfigurationFactory();
    m_gameWorldEditorFactory = new GameWorldEditorFactory();
    m_gameObjectPropertyViewFactory = new GameObjectPropertyViewFactory();

    ProjectExplorer::ProjectManager::registerProjectType<Project>(VOLCANO_EDITOR_PROJECT_MIMETYPE);

    return true;
}

VOLCANO_EDITOR_END
