//
//
#include <Volcano/Graphics/Surface.hpp>

VOLCANO_GRAPHICS_BEGIN

Surface::Surface(void):
    m_activated(false)
{
}

Surface::~Surface(void)
{
    VOLCANO_ASSERT(!m_activated);
}

bool Surface::isValid(void)
{
    return false;
}

void Surface::resize(int width, int height)
{
    m_size.set(width, height);
}

bool Surface::activate(void)
{
    VOLCANO_ASSERT(!m_activated);

    m_activated = true;

	return true;
}

void Surface::deactivate(void)
{
    VOLCANO_ASSERT(m_activated);

    m_activated = false;
}

VOLCANO_GRAPHICS_END
