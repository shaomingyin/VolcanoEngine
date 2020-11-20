//
//
#ifndef VOLCANO_ASYNC_HPP
#define VOLCANO_ASYNC_HPP

#include <atomic>
#include <thread>
#include <functional>

#include <Volcano/Common.hpp>
#include <Volcano/Object.hpp>

VOLCANO_BEGIN

class Async: public Object {
public:
    Async(Callback cb, Object *parent = nullptr);
    ~Async(void) override;

public:
    void notify(void);

protected:
    Duration onUpdate(void) override;

private:
    enum State {
        STATE_IDLE = 0,
        STATE_ACTIVATING,
        STATE_PENDING
    };

    Callback m_cb;
    std::atomic<State> m_state;
};

VOLCANO_INLINE void Async::notify(void)
{
    VOLCANO_ASSERT(std::this_thread::get_id() != threadId());

    State exp = STATE_IDLE;
    while (!m_state.compare_exchange_strong(exp, STATE_ACTIVATING));
    kick();
}

VOLCANO_END

#endif // VOLCANO_ASYNC_HPP
