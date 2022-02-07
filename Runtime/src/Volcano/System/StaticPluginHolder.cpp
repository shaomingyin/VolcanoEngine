//
//
#include <Volcano/System/StaticPluginHolder.hpp>

VOLCANO_SYSTEM_BEGIN

StaticPluginHolder::StaticPluginHolder(void):
    m_plugin(nullptr)
{
}

StaticPluginHolder::~StaticPluginHolder(void)
{
}

bool StaticPluginHolder::load(QStaticPlugin *staticPlugin)
{
    Q_ASSERT(m_plugin == nullptr);
    Q_ASSERT(staticPlugin != nullptr);

    auto instance = staticPlugin->instance();
    auto plugin = qobject_cast<Plugin *>(instance);
    if (plugin == nullptr)
        return false;

    if (!init(plugin, staticPlugin->metaData()))
        return false;

    m_plugin = staticPlugin;

    return true;
}

VOLCANO_SYSTEM_END
