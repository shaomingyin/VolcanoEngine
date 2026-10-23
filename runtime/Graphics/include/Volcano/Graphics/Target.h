#ifndef VOLCANO_GRAPHICS_TARGET_H
#define VOLCANO_GRAPHICS_TARGET_H

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Renderer;

class Target {
public:
    virtual ~Target() = default;
    Target(const Target&) = delete;
    Target& operator=(const Target&) = delete;

    virtual int width() const noexcept = 0;
    virtual int height() const noexcept = 0;

protected:
    Target() = default;

private:
    friend class Renderer;
    virtual std::uint32_t framebuffer() const noexcept = 0;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_TARGET_H
