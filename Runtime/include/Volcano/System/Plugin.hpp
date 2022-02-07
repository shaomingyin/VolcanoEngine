//
//
#ifndef VOLCANO_SYSTEM_PLUGIN_HPP
#define VOLCANO_SYSTEM_PLUGIN_HPP

#include <Volcano/System/Common.hpp>
#include <Volcano/System/Core.hpp>

VOLCANO_SYSTEM_BEGIN

class Plugin {
public:
    Plugin(void);
    virtual ~Plugin(void);

public:
    bool start(Core *core);
    void stop(void);
    bool isStarted(void) const;

protected:
    virtual bool init(Core *core) = 0;
    virtual void shutdown(void) = 0;

private:
    bool m_isStarted;
};

VOLCANO_SYSTEM_END

Q_DECLARE_INTERFACE(Volcano::System::Plugin, "Volcano.System.Plugin")

#endif // VOLCANO_SYSTEM_PLUGIN_HPP
