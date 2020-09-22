//
//
#include <Volcano/OpenGL/Context.hpp>

VOLCANO_OPENGL_BEGIN

Context *Context::c_current = nullptr;

Context::Context(void)
{
}

Context::~Context(void)
{
}

bool Context::init(void)
{
    if (!m_renderer.init())
        return false;

    return true;
}

VOLCANO_OPENGL_END
