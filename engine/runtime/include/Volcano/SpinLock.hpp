//
//
#ifndef VOLCANO_SPINLOCK_HPP
#define VOLCANO_SPINLOCK_HPP

#include <QAtomicInt>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class SpinLock final {
public:
    SpinLock(void);
    ~SpinLock(void);

public:
    void lock(void);
    bool tryLock(void);
    void unlock(void);
    bool isLocked(void) const;

private:
    QAtomicInt m_value;
};

VOLCANO_INLINE SpinLock::SpinLock(void)
{
}

VOLCANO_INLINE SpinLock::~SpinLock(void)
{
    Q_ASSERT(!isLocked());
}

VOLCANO_INLINE void SpinLock::lock(void)
{
    while (!m_value.testAndSetRelaxed(0, 1));
}

VOLCANO_INLINE bool SpinLock::tryLock(void)
{
    return m_value.testAndSetRelaxed(0, 1);
}

VOLCANO_INLINE void SpinLock::unlock(void)
{
    Q_ASSERT(m_value == 1);

    m_value.storeRelaxed(0);
}

VOLCANO_INLINE bool SpinLock::isLocked(void) const
{
    return (m_value == 1);
}

VOLCANO_END

#endif // VOLCANO_SPINLOCK_HPP
