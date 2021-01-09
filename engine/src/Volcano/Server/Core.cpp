//
//
#include <Volcano/ScopeGuard.hpp>
#include <Volcano/Server/Core.hpp>

VOLCANO_SERVER_BEGIN

Core::Core(uv_loop_t *loop):
    m_loop(loop),
    m_started(false)
{
    VOLCANO_ASSERT(m_loop != nullptr);
}

Core::~Core(void)
{
    if (m_started)
        stop();
}

bool Core::start(void)
{
    VOLCANO_ASSERT(!m_started);

    return true;
}

void Core::stop(void)
{
    VOLCANO_ASSERT(m_started);
    m_started = false;
}

VOLCANO_SERVER_END
