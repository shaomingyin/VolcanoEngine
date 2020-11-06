//
//
#ifndef VOLCANO_SPINLOCK_HPP
#define VOLCANO_SPINLOCK_HPP

#include <QAtomicInt>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class SpinLock final
{
public:
    SpinLock(void);
    ~SpinLock(void);

public:
    bool tryLock(void);
    void lock(void);
    void unlock(void);

private:
    QAtomicInt m_value;
};

VOLCANO_INLINE SpinLock::SpinLock(void):
    m_value(0)
{
}

VOLCANO_INLINE SpinLock::~SpinLock(void)
{
    Q_ASSERT(m_value.loadRelaxed() == 0);
}

VOLCANO_INLINE bool SpinLock::tryLock(void)
{
    return m_value.testAndSetRelaxed(0, 1);
}

VOLCANO_INLINE void SpinLock::lock(void)
{
    while (m_value.testAndSetRelaxed(0, 1));
}

VOLCANO_INLINE void SpinLock::unlock(void)
{
    Q_ASSERT(m_value.loadRelaxed() == 1);

    m_value.storeRelaxed(0);
}

class ScopedSpinLocker final
{
public:
    ScopedSpinLocker(SpinLock &lock);
    ~ScopedSpinLocker(void);

private:
    SpinLock &m_lock;
};

VOLCANO_INLINE ScopedSpinLocker::ScopedSpinLocker(SpinLock &lock):
    m_lock(lock)
{
    m_lock.lock();
}

VOLCANO_INLINE ScopedSpinLocker::~ScopedSpinLocker(void)
{
    m_lock.unlock();
}

VOLCANO_END

#endif // VOLCANO_SPINLOCK_HPP
