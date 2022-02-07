//
//
#include <memory>

#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>

#include <Volcano/System/Engine.hpp>

VOLCANO_SYSTEM_BEGIN

Engine::Engine(QObject *parent):
    Core(parent)
{
}

Engine::~Engine(void)
{
}

bool Engine::init(const QUrl &url)
{
    if (!Core::init(url))
        return false;

    loadAllPlugins();

    for (auto &pluginHolder: m_pluginLoaderList) {
        if (!pluginHolder.start(this)) {
            // TODO
        }
    }

    return true;
}

bool Engine::addPluginPath(const QString &pathName)
{
    QFileInfo fi(pathName);
    if (!fi.isDir())
        return false;

    auto absolutePath = fi.absolutePath();
    if (m_pluginPathList.contains(absolutePath))
        return false;

    m_pluginPathList.append(absolutePath);
    loadPlugins(absolutePath);

    return true;
}

void Engine::loadAllPlugins(void)
{
    auto staticPlugins = QPluginLoader::staticPlugins();
    for (auto &staticPlugin: staticPlugins) {

    }

    for (auto path: m_pluginPathList)
        loadPlugins(path);

    m_pluginLoaderList.sort();

}

bool Engine::loadPlugins(const QString &pathName)
{
    QDir dir(pathName);
    QFileInfoList entryInfoList = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::Readable | QDir::NoDotAndDotDot);
    for (auto fi: entryInfoList) {
        if (fi.isDir())
            loadPlugins(fi.absolutePath());
        else
            loadPlugin(fi.absolutePath());
    }
}

bool Engine::loadPlugin(const QString fileName)
{
    auto pluginLoader = std::make_unique<QPluginLoader>(fileName);
    if (!pluginLoader || !pluginLoader->load())
        return false;

    m_pluginLoaderList.push_back(pluginLoader.release());

    return true;
}

VOLCANO_SYSTEM_END
