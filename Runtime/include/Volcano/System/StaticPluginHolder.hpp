//
//
#ifndef VOLCANO_SYSTEM_STATICPLUGINHOLDER_HPP
#define VOLCANO_SYSTEM_STATICPLUGINHOLDER_HPP

#include <QStaticPlugin>

#include <Volcano/System/Common.hpp>
#include <Volcano/System/PluginHolder.hpp>

VOLCANO_SYSTEM_BEGIN

class StaticPluginHolder: public PluginHolder {
public:
    StaticPluginHolder(void);
    ~StaticPluginHolder(void) override;

public:
    bool load(QStaticPlugin *staticPlugin);

private:
    QStaticPlugin *m_plugin;
};

VOLCANO_SYSTEM_END

#endif // VOLCANO_SYSTEM_STATICPLUGINHOLDER_HPP
