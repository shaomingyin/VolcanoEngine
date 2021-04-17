//
//
#ifndef VOLCANO_GRAPHICS_BUFFER_HPP
#define VOLCANO_GRAPHICS_BUFFER_HPP

#include <Volcano/IO.hpp>
#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class Buffer: public IO {
public:
    Buffer(void);
    ~Buffer(void) override;

public:
    virtual void bind(void) = 0;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_BUFFER_HPP
