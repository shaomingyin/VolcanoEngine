//
//
#ifndef VOLCANO_OPENGL_CONTEXT_HPP
#define VOLCANO_OPENGL_CONTEXT_HPP

#include <Volcano/OpenGL/Common.hpp>
#include <Volcano/OpenGL/Memory.hpp>
#include <Volcano/OpenGL/Renderer.hpp>

VOLCANO_OPENGL_BEGIN

class Context
{
public:
    Context(void);
    virtual ~Context(void);

public:
    bool init(void);
    void makeCurrent(void);
    static Context *current(void);
    Memory &memory(void);
    Renderer &renderer(void);

private:
    Memory m_memory;
    Renderer m_renderer;
    static Context *c_current;
};

VOLCANO_INLINE void Context::makeCurrent(void)
{
    c_current = this;
}

VOLCANO_INLINE Context *Context::current(void)
{
    return c_current;
}

VOLCANO_INLINE Memory &Context::memory(void)
{
    return m_memory;
}

VOLCANO_INLINE Renderer &Context::renderer(void)
{
    return m_renderer;
}

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_CONTEXT_HPP
