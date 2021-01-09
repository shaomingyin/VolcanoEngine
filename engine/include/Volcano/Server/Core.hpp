//
//
#ifndef VOLCANO_SERVER_CORE_HPP
#define VOLCANO_SERVER_CORE_HPP

#include <string>
#include <thread>
#include <future>

#include <Volcano/Server/Common.hpp>
#include <Volcano/Server/World.hpp>

VOLCANO_SERVER_BEGIN

class Core {
public:
    Core(uv_loop_t *loop);
    virtual ~Core(void);

public:
    virtual bool start(void);
    virtual void stop(void);
    uv_loop_t *loop(void);

private:
    uv_loop_t *m_loop;
    bool m_started;
    uv_async_t m_renderAsync;
};

VOLCANO_INLINE uv_loop_t *Core::loop(void)
{
    return m_loop;
}

VOLCANO_SERVER_END

#endif // VOLCANO_SERVER_CORE_HPP
