//
//
#ifndef VOLCANO_DRIVERS_GRAPHICS_BUFFER_HPP
#define VOLCANO_DRIVERS_GRAPHICS_BUFFER_HPP

#include <Volcano/IO.hpp>
#include <Volcano/Drivers/Graphics/Common.hpp>

VOLCANO_DRIVERS_GRAPHICS_BEGIN

class Buffer: public IO {
public:
    Buffer(void);
    ~Buffer(void) override;

public:
    virtual void bind(void) = 0;
};

VOLCANO_DRIVERS_GRAPHICS_END

#endif // VOLCANO_DRIVERS_GRAPHICS_BUFFER_HPP
