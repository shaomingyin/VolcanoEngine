//
//
#ifndef VOLCANO_CLIENT_GRAPHICS_FRAMEBUFFER_HPP
#define VOLCANO_CLIENT_GRAPHICS_FRAMEBUFFER_HPP

#include <Volcano/Client/Graphics/Common.hpp>

VOLCANO_CLIENT_GRAPHICS_BEGIN

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

VOLCANO_CLIENT_GRAPHICS_END

#endif // VOLCANO_CLIENT_GRAPHICS_FRAMEBUFFER_HPP
