//
//
#ifndef VOLCANO_GRAPHICS_FRAMEBUFFER_HPP
#define VOLCANO_GRAPHICS_FRAMEBUFFER_HPP

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Surface.hpp>

VOLCANO_GRAPHICS_BEGIN

class FrameBuffer: public Surface {
public:
    FrameBuffer(void);
    ~FrameBuffer(void) override;

public:
    GLuint id(void) const;
	void resize(int width, int height) override;

private:
    GLuint m_id;
};

VOLCANO_INLINE GLuint FrameBuffer::id(void) const
{
    return m_id;
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_FRAMEBUFFER_HPP
