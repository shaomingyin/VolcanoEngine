//
//
#include <QScopeGuard>

#include <Volcano/System/PluginHolder.hpp>

VOLCANO_SYSTEM_BEGIN

PluginHolder::PluginHolder(void):
    m_plugin(nullptr)
{
}

PluginHolder::~PluginHolder(void)
{
}

bool PluginHolder::start(Core *core)
{
    Q_ASSERT(core != nullptr);

    if (m_plugin != nullptr && !m_plugin->isStarted())
        return m_plugin->start(core);

    return false;
}

void PluginHolder::stop(void)
{
    if (m_plugin == nullptr || !m_plugin->isStarted())
        return;

    m_plugin->stop();
}

bool PluginHolder::isStarted(void) const
{
    return (m_plugin != nullptr && m_plugin->isStarted());
}

const QVersionNumber &PluginHolder::version(void) const
{
    Q_ASSERT(m_plugin != nullptr);

    return m_version;
}

const QString &PluginHolder::name(void) const
{
    Q_ASSERT(m_plugin != nullptr);

    return m_name;
}

const QString &PluginHolder::description(void) const
{
    Q_ASSERT(m_plugin != nullptr);

    return m_description;
}

bool PluginHolder::operator<(const PluginHolder &that) const
{
    // TODO
    return true;
}

bool PluginHolder::init(Plugin *plugin, QJsonObject metaData)
{
    Q_ASSERT(plugin != nullptr);
    Q_ASSERT(m_plugin == nullptr);

    auto version = metaData.value("version");
    if (version.isNull())
        return false;

    auto name = metaData.value("name");
    if (name.isNull())
        return false;

    auto description = metaData.value("description");
    if (description.isNull())
        return false;

    auto dependencies = metaData.value("dependencies");
    if (dependencies.isArray())
        return false;

    DependencyMap dependencyMap;
    auto dependenciesArray = dependencies.toArray();

    for (int i = 0; i < dependenciesArray.size(); ++i) {
        auto dependency = dependenciesArray.at(i);
        if (!dependency.isObject())
            return false;
        if (!populateDependency(dependency.toObject(), dependencyMap))
            return false;
    }

    m_plugin = plugin;
    m_version = QVersionNumber::fromString(version.toString());
    m_name = name.toString();
    m_description = description.toString();
    m_dependencyMap = std::move(dependencyMap);

    return true;
}

bool PluginHolder::populateDependency(QJsonObject dependency, DependencyMap &out)
{
    auto nameValue = dependency.value("name");
    if (!nameValue.isString())
        return false;

    auto versionValue = dependency.value("version");
    if (!versionValue.isString())
        return false;

    auto nameString = nameValue.toString();
    auto versionString = versionValue.toString();
    auto version = QVersionNumber::fromString(versionString);
    auto targetVersion = out.value(nameString, QVersionNumber::fromString("0.0.0"));
    if (version > targetVersion)
        out[nameString] = version;

    return true;
}

VOLCANO_SYSTEM_END
