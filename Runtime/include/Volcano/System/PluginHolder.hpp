//
//
#ifndef VOLCANO_SYSTEM_PLUGINHOLDER_HPP
#define VOLCANO_SYSTEM_PLUGINHOLDER_HPP

#include <QMap>
#include <QString>
#include <QVersionNumber>
#include <QJsonObject>

#include <Volcano/System/Common.hpp>
#include <Volcano/System/Core.hpp>
#include <Volcano/System/Plugin.hpp>

VOLCANO_SYSTEM_BEGIN

class PluginHolder {
public:
    PluginHolder(void);
    virtual ~PluginHolder(void);

public:
    bool start(Core *core);
    void stop(void);
    bool isStarted(void) const;
    const QVersionNumber &version(void) const;
    const QString &name(void) const;
    const QString &description(void) const;
    bool operator<(const PluginHolder &that) const;

protected:
    bool init(Plugin *plugin, QJsonObject metaData);

private:
    using DependencyMap = QMap<QString, QVersionNumber>;
    bool populateDependency(QJsonObject dependency, DependencyMap &out);

private:
    Plugin *m_plugin;
    QVersionNumber m_version;
    QString m_name;
    QString m_description;
    DependencyMap m_dependencyMap;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_PLUGINHOLDER_HPP
