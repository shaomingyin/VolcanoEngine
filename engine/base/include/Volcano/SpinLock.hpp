//
//
#ifndef VOLCANO_SPINLOCK_HPP
#define VOLCANO_SPINLOCK_HPP

#include <QAtomicInt>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class VOLCANO_API SpinLock final {
public:
    SpinLock(void);
    ~SpinLock(void);

public:
    bool tryLock(void);
    void lock(void);
    void unlock(void);
    bool isLocked(void) const;

private:
    QAtomicInt m_state;
};

VOLCANO_INLINE SpinLock::SpinLock(void)
{
}

VOLCANO_INLINE SpinLock::~SpinLock(void)
{
    Q_ASSERT(!isLocked());
}

VOLCANO_INLINE bool SpinLock::tryLock(void)
{
    return m_state.testAndSetAcquire(0, 1);
}

VOLCANO_INLINE void SpinLock::lock(void)
{
    while (!m_state.testAndSetAcquire(0, 1));
}

VOLCANO_INLINE void SpinLock::unlock(void)
{
    m_state.testAndSetAcquire(1, 0);
}

VOLCANO_INLINE bool SpinLock::isLocked(void) const
{
    return m_state.loadAcquire();
}

VOLCANO_END

#endif // VOLCANO_SPINLOCK_HPP
