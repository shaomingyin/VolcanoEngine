//
//
#ifndef VOLCANO_VIEW_HPP
#define VOLCANO_VIEW_HPP

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class View
{
public:
    View(void);
    virtual ~View(void);

public:
    void reset(void);
};

VOLCANO_INLINE void View::reset(void)
{
}

VOLCANO_END

#endif // VOLCANO_VIEW_HPP
