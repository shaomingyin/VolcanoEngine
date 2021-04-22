//
//
#ifndef VOLCANO_GRPHICS_RENDERER_HPP
#define VOLCANO_GRPHICS_RENDERER_HPP

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Mesh.hpp>
#include <Volcano/Graphics/Material.hpp>
#include <Volcano/Graphics/Light.hpp>
#include <Volcano/Graphics/DirectionalLight.hpp>
#include <Volcano/Graphics/PointLight.hpp>
#include <Volcano/Graphics/SpotLight.hpp>

VOLCANO_GRAPHICS_BEGIN

class Renderer {
public:
    enum MatrixMode {
        MatrixModeModel = 0,
        MatrixModeView,
        MatrixModeProjection,
        MatrixModeMax
    };

public:
    Renderer(void);
    virtual ~Renderer(void);

public:
    bool init(int x, int y, int width, int height);
    const Eigen::Vector4i &viewport(void) const;
    void setViewport(int x, int y, int width, int height);
    Light &ambientLight(void);
    void update(void);

private:
    Eigen::Vector4i m_viewport;
    Light m_ambientLight;
    GLEXContext *m_glex;
    NVGcontext *m_nvg;
};

VOLCANO_INLINE Light &Renderer::ambientLight(void)
{
    return m_ambientLight;
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRPHICS_RENDERER_HPP
