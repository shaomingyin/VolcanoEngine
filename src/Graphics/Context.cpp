//
//
#include <Volcano/Graphics/Context.hpp>

VOLCANO_GRAPHICS_BEGIN

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

VOLCANO_GRAPHICS_END
