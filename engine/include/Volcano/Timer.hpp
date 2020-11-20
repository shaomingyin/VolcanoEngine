//
//
#ifndef VOLCANO_TIMER_HPP
#define VOLCANO_TIMER_HPP

#include <Volcano/Common.hpp>
#include <Volcano/Object.hpp>

VOLCANO_BEGIN

class Timer: public Object {
public:
    Timer(Callback cb, Object *parent = nullptr);
    ~Timer(void) override;

public:
    bool enable(Duration timeout, Duration repeat = Duration::min());

protected:
    Duration onUpdate(void) override;

private:
    Callback m_cb;
    Duration m_repeat;
    TimePoint m_expired;
};

VOLCANO_END

#endif // VOLCANO_TIMER_HPP
