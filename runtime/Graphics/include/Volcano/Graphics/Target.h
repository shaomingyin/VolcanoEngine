#ifndef VOLCANO_GRAPHICS_TARGET_H
#define VOLCANO_GRAPHICS_TARGET_H

#include <Volcano/Size.h>
#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Texture.h>

VOLCANO_GRAPHICS_BEGIN

// Implementations provide a color output and depth storage for the built-in renderer.
class Target {
public:
    virtual ~Target() = default;
    Target(const Target&) = delete;
    Target& operator=(const Target&) = delete;

    // Selects this output in the caller's current OpenGL context.
    virtual void bind() const noexcept = 0;

    virtual int width() const noexcept = 0;
    virtual int height() const noexcept = 0;

    // Texture-backed targets override this query to prevent sampling their own attachments.
    virtual bool contains(const Texture&) const noexcept {
        return false;
    }

    Size size() const noexcept {
        return {width(), height()};
    }

protected:
    Target() = default;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_TARGET_H
