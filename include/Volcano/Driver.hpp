//
//
#ifndef VOLCANO_DRIVER_HPP
#define VOLCANO_DRIVER_HPP

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class VOLCANO_API Driver
{
public:
    Driver(void);
    virtual ~Driver(void);

public:
    virtual bool init(void) = 0;
    virtual void shutdown(void) = 0;
    virtual void frame(float elapsed) = 0;
};

VOLCANO_END

#endif // VOLCANO_DRIVER_HPP
