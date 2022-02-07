//
//
#ifndef VOLCANO_SYSTEM_ENGINE_HPP
#define VOLCANO_SYSTEM_ENGINE_HPP

#include <list>

#include <QStringList>

#include <Volcano/System/Common.hpp>
#include <Volcano/System/Core.hpp>
#include <Volcano/System/Plugin.hpp>
#include <Volcano/System/PluginHolder.hpp>

VOLCANO_SYSTEM_BEGIN

using PluginHolderList = std::list<PluginHolder>;

class Engine: public Core {
    Q_OBJECT

public:
    Engine(QObject *parent = nullptr);
    ~Engine(void) override;

public:
    bool init(const QUrl &url) override;
    bool addPluginPath(const QString &pathName);

protected:
    void loadAllPlugins(void);
    bool loadPlugins(const QString &pathName);
    bool loadPlugin(const QString fileName);

private:
    QStringList m_pluginPathList;
    PluginHolderList m_pluginLoaderList;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_ENGINE_HPP
