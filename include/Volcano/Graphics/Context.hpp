//
//
#ifndef VOLCANO_GRAPHICS_CONTEXT_HPP
#define VOLCANO_GRAPHICS_CONTEXT_HPP

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Memory.hpp>
#include <Volcano/Graphics/Renderer.hpp>

VOLCANO_GRAPHICS_BEGIN

class VOLCANO_API Context
{
public:
    Context(void);
    virtual ~Context(void);

public:
    bool init(void);
    Memory &memory(void);
    Renderer &renderer(void);

private:
    Memory m_memory;
    Renderer m_renderer;
};

VOLCANO_INLINE Memory &Context::memory(void)
{
    return m_memory;
}

VOLCANO_INLINE Renderer &Context::renderer(void)
{
    return m_renderer;
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_CONTEXT_HPP
