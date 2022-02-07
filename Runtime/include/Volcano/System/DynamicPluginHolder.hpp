//
//
#ifndef VOLCANO_SYSTEM_DYNAMICPLUGINHOLDER_HPP
#define VOLCANO_SYSTEM_DYNAMICPLUGINHOLDER_HPP

#include <QString>
#include <QPluginLoader>

#include <Volcano/System/Common.hpp>
#include <Volcano/System/Core.hpp>
#include <Volcano/System/PluginHolder.hpp>

VOLCANO_SYSTEM_BEGIN

class DynamicPluginHolder: public PluginHolder {
public:
    DynamicPluginHolder(void);
    ~DynamicPluginHolder(void) override;

public:
    bool load(const QString &fileName);

private:
    QPluginLoader m_loader;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_DYNAMICPLUGINHOLDER_HPP
