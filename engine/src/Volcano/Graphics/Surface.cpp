//
//
#include <Volcano/Graphics/Surface.hpp>

VOLCANO_GRAPHICS_BEGIN

Surface::Surface(void)
{
}

Surface::~Surface(void)
{
}

bool Surface::isValid(void)
{
    return false;
}

void Surface::resize(int width, int height)
{
    m_size.set(width, height);
}

bool Surface::begin(void)
{
	return true;
}

void Surface::end(void)
{
}

VOLCANO_GRAPHICS_END
