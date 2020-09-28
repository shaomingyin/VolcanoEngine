//
//
#include <Volcano/Graphics/OpenGL/Target.hpp>

VOLCANO_GRAPHICS_OPENGL_BEGIN

Target::Target(void)
{
}

Target::Target(int width, int height):
    Graphics::Target(width, height)
{
}

Target::~Target(void)
{
}

bool Target::init(void)
{
    return true;
}

void Target::shutdown(void)
{
}

void Target::onSizeChanged(void)
{
}

VOLCANO_GRAPHICS_OPENGL_END
