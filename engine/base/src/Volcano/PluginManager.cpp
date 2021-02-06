//
//
#include <Volcano/PluginManager.hpp>

VOLCANO_BEGIN

PluginManager::PluginManager(QObject *parent):
    QObject(parent)
{
}

PluginManager::~PluginManager(void)
{
}

bool PluginManager::init(void)
{
    return true;
}

VOLCANO_END
