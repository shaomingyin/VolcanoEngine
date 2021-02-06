//
//
#ifndef VOLCANO_PLUGIN_HPP
#define VOLCANO_PLUGIN_HPP

#include <QtPlugin>

#include <QString>
#include <QVersionNumber>

#include <Volcano/Common.hpp>
#include <Volcano/Context.hpp>

VOLCANO_BEGIN

class Plugin {
public:
    virtual bool init(Context *context) = 0;
    virtual void shutdown(void) = 0;
    virtual const QString &name(void) const = 0;
    virtual const QVersionNumber &version(void) const = 0;
    virtual const QString &description(void) const = 0;
};

VOLCANO_END

Q_DECLARE_INTERFACE(Volcano::Plugin, "volcano.plugin")

#endif // VOLCANO_PLUGIN_HPP
