//
//
#ifndef VOLCANO_PLUGINMANAGER_HPP
#define VOLCANO_PLUGINMANAGER_HPP

#include <QList>
#include <QObject>
#include <QPluginLoader>

#include <Volcano/Common.hpp>
#include <Volcano/Plugin.hpp>

VOLCANO_BEGIN

class PluginManager: public QObject
{
    Q_OBJECT

public:
    PluginManager(QObject *parent = nullptr);
    ~PluginManager(void) override;

public:
    bool init(void);

private:
    struct PluginInfo {
        QPluginLoader loader;
        Plugin *plugin;
    };

    using PluginList = QList<PluginInfo>;

private:
    PluginList m_pluginList;
};

VOLCANO_END

#endif // VOLCANO_PLUGINMANAGER_HPP
