//
//
#include <Volcano/System/Plugin.hpp>

VOLCANO_SYSTEM_BEGIN

Plugin::Plugin(void):
    m_isStarted(false)
{
}

Plugin::~Plugin(void)
{
}

bool Plugin::start(Core *core)
{
    Q_ASSERT(core != nullptr);
    Q_ASSERT(!m_isStarted);

    m_isStarted = init(core);

    return m_isStarted;
}

void Plugin::stop(void)
{
    Q_ASSERT(m_isStarted);

    shutdown();
    m_isStarted = false;
}

bool Plugin::isStarted(void) const
{
    return m_isStarted;
}

VOLCANO_SYSTEM_END
