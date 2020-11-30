//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_HPP
#define VOLCANO_GRAPHICS_RENDERER_HPP

#include <nanovg.h>

#include <GL/Program.hpp>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Surface.hpp>
#include <Volcano/Graphics/Light.hpp>
#include <Volcano/Graphics/Mesh.hpp>
#include <Volcano/Graphics/Material.hpp>
#include <Volcano/Graphics/View.hpp>
#include <Volcano/Graphics/DebugDrawRenderer.hpp>

VOLCANO_GRAPHICS_BEGIN

class Renderer {
public:
    Renderer(void);
    virtual ~Renderer(void);

public:
    bool init(Surface *target);
	void shutdown(void);
    void render(const View &v);
    Surface *target(void);

private:
    Surface *m_target;
    NVGcontext *m_nvg;
    dd::ContextHandle m_ddContext;
    DebugDrawRenderer m_ddRenderer;
    GL::Program m_program;
    Light m_ambientLight;
};

VOLCANO_INLINE Surface *Renderer::target(void)
{
    return m_target;
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_HPP
