//
//
#include <QScopeGuard>

#include <Volcano/System/DynamicPluginHolder.hpp>

VOLCANO_SYSTEM_BEGIN

DynamicPluginHolder::DynamicPluginHolder(void)
{
}

DynamicPluginHolder::~DynamicPluginHolder(void)
{
    if (m_loader.isLoaded())
        m_loader.unload();
}

bool DynamicPluginHolder::load(const QString &fileName)
{
    Q_ASSERT(m_loader.isLoaded());

    m_loader.setFileName(fileName);
    if (!m_loader.load())
        return false;

    QScopeGuard unloader([this] { m_loader.unload(); });

    auto instance = m_loader.instance();
    auto plugin = qobject_cast<Plugin *>(instance);
    if (plugin == nullptr)
        return false;

    if (!init(plugin, m_loader.metaData()))
        return false;

    unloader.dismiss();

    return true;
}

VOLCANO_SYSTEM_END
