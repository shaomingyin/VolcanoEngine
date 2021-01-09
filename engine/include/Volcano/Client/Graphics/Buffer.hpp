//
//
#ifndef VOLCANO_CLIENT_GRAPHICS_BUFFER_HPP
#define VOLCANO_CLIENT_GRAPHICS_BUFFER_HPP

#include <Volcano/Client/Graphics/Common.hpp>

VOLCANO_CLIENT_GRAPHICS_BEGIN

class Buffer {
public:
    Buffer(void);
    ~Buffer(void);

public:
    virtual void bind(void) = 0;
};

VOLCANO_CLIENT_GRAPHICS_END

#endif // VOLCANO_CLIENT_GRAPHICS_BUFFER_HPP
