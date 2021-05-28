//
//
#ifndef VOLCANO_GRAPHICS_BUFFER_HPP
#define VOLCANO_GRAPHICS_BUFFER_HPP

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class Buffer {
public:
    Buffer(void);
    virtual ~Buffer(void);

public:
    virtual GLsizeiptr size(void) = 0;
    virtual void bind(void) = 0;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_BUFFER_HPP
