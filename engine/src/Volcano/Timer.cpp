//
//
#include <Volcano/Timer.hpp>

VOLCANO_BEGIN

Timer::Timer(Callback cb, Object *parent):
    Object(parent),
    m_cb(cb)
{
    VOLCANO_ASSERT(m_cb);
}

Timer::~Timer(void)
{
}

bool Timer::enable(Duration timeout, Duration repeat)
{
    VOLCANO_ASSERT(!isEnabled());

    m_expired = now() + timeout;
    m_repeat = repeat;

    Object::enable();

    return true;
}

Duration Timer::onUpdate(void)
{
    Duration timeout = Duration::max();
    auto curr = now();

    if (curr >= m_expired) {
        m_cb();
        if (m_repeat > Duration::min()) {
            m_expired = curr + m_repeat;
            timeout = m_repeat;
        } else
            disable();
    }

    return timeout;
}

VOLCANO_END
