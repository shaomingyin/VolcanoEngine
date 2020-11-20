//
//
#include <Volcano/Async.hpp>

VOLCANO_BEGIN

Async::Async(Callback cb, Object *parent):
    Object(parent),
    m_cb(cb)
{
    VOLCANO_ASSERT(m_cb);

    enable();
}

Async::~Async(void)
{
}

Duration Async::onUpdate(void)
{
    State exp = STATE_ACTIVATING;
    if (m_state.compare_exchange_strong(exp, STATE_PENDING)) {
        m_state = STATE_IDLE;
        return Duration::min();
    }
    return Duration::max();
}

VOLCANO_END
