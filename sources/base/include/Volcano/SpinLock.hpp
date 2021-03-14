//
//
#ifndef VOLCANO_SPINLOCK_HPP
#define VOLCANO_SPINLOCK_HPP

#include <atomic>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class SpinLock final {
public:
    SpinLock(void);

public:
    bool tryLock(void);
    void lock(void);
    void unlock(void);

private:
    std::atomic_flag m_flag;
};

VOLCANO_INLINE SpinLock::SpinLock(void)
{
    m_flag.clear();
}

VOLCANO_INLINE bool SpinLock::tryLock(void)
{
    return m_flag.test_and_set();
}

VOLCANO_INLINE void SpinLock::lock(void)
{
    while (!m_flag.test_and_set());
}

VOLCANO_INLINE void SpinLock::unlock(void)
{
    m_flag.clear();
}

VOLCANO_END

#endif // VOLCANO_SPINLOCK_HPP
