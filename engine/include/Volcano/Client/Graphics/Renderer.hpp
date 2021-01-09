//
//
#ifndef VOLCANO_CLIENT_GRAPHICS_RENDERER_HPP
#define VOLCANO_CLIENT_GRAPHICS_RENDERER_HPP

#include <nanovg.h>

#include <GL/Program.hpp>

#include <Volcano/Client/Graphics/Common.hpp>
#include <Volcano/Client/Graphics/Surface.hpp>
#include <Volcano/Client/Graphics/Light.hpp>
#include <Volcano/Client/Graphics/Mesh.hpp>
#include <Volcano/Client/Graphics/Material.hpp>
#include <Volcano/Client/Graphics/View.hpp>
#include <Volcano/Client/Graphics/DebugDrawRenderer.hpp>

VOLCANO_CLIENT_GRAPHICS_BEGIN

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

VOLCANO_CLIENT_GRAPHICS_END

#endif // VOLCANO_CLIENT_GRAPHICS_RENDERER_HPP
