//
//
#ifndef VOLCANO_UI_GRAPHICS_BUFFER_HPP
#define VOLCANO_UI_GRAPHICS_BUFFER_HPP

#include <Volcano/IO.hpp>
#include <Volcano/UI/Graphics/Common.hpp>

VOLCANO_UI_GRAPHICS_BEGIN

class Buffer: public IO {
public:
    Buffer(void);
    ~Buffer(void) override;

public:
    virtual void bind(void) = 0;
};

VOLCANO_UI_GRAPHICS_END

#endif // VOLCANO_UI_GRAPHICS_BUFFER_HPP
