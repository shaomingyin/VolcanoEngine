//
//
#ifndef VOLCANO_GRAPHICS_OPENGL_TARGET_HPP
#define VOLCANO_GRAPHICS_OPENGL_TARGET_HPP

#include <QSize>

#include <Volcano/Graphics/Target.hpp>
#include <Volcano/Graphics/OpenGL/Common.hpp>

VOLCANO_GRAPHICS_OPENGL_BEGIN

class VOLCANO_API Target: public Graphics::Target
{
public:
    Target(void);
    Target(int width, int height);
    ~Target(void) override;

public:
    bool init(void);
    void shutdown(void);

protected:
    void onSizeChanged(void) override;
};

VOLCANO_GRAPHICS_OPENGL_END

#endif // VOLCANO_GRAPHICS_OPENGL_TARGET_HPP
