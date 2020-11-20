//
//
#include <Volcano/Graphics/FrameBuffer.hpp>

VOLCANO_GRAPHICS_BEGIN

FrameBuffer::FrameBuffer(void):
	m_id(0)
{
}

FrameBuffer::~FrameBuffer(void)
{
}

void FrameBuffer::resize(int width, int height)
{
    // TODO

    Surface::resize(width, height);
}

VOLCANO_GRAPHICS_END
