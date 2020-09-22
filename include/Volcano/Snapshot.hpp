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
    Snapshot(void);
    virtual ~Snapshot(void);

public:
    View &view(void);
    const View &view(void) const;

private:
    View m_view;
};

VOLCANO_INLINE View &Snapshot::view(void)
{
    return m_view;
}

VOLCANO_INLINE const View &Snapshot::view(void) const
{
    return m_view;
}

VOLCANO_END

#endif // VOLCANO_SNAPSHOT_HPP
