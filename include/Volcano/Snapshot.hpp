//
//
#ifndef VOLCANO_SNAPSHOT_HPP
#define VOLCANO_SNAPSHOT_HPP

#include <Volcano/Common.hpp>
#include <Volcano/View.hpp>

VOLCANO_BEGIN

class VOLCANO_API Snapshot
{
public:
    enum
    {
        FlagView = 0x1
    };

public:
    Snapshot(void);
    virtual ~Snapshot(void);

public:
    int flags(void) const;
    void setFlags(int v);
    View &view(void);
    const View &view(void) const;

private:
    int m_flags;
    View m_view;
};

VOLCANO_INLINE int Snapshot::flags(void) const
{
    return m_flags;
}

VOLCANO_INLINE void Snapshot::setFlags(int v)
{
    m_flags = v;
}

VOLCANO_INLINE View &Snapshot::view(void)
{
    Q_ASSERT(m_flags & FlagView);
    return m_view;
}

VOLCANO_INLINE const View &Snapshot::view(void) const
{
    Q_ASSERT(m_flags & FlagView);
    return m_view;
}

VOLCANO_END

#endif // VOLCANO_SNAPSHOT_HPP
