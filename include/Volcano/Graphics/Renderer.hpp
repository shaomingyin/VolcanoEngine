//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_HPP
#define VOLCANO_GRAPHICS_RENDERER_HPP

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Target.hpp>
#include <Volcano/Graphics/Mesh.hpp>

VOLCANO_GRAPHICS_BEGIN

class VOLCANO_API Renderer
{
public:
    Renderer(void);
    virtual ~Renderer(void);

public:
    virtual bool init(void) = 0;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_HPP
