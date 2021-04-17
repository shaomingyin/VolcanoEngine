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
    bool init(int width, int height);
    void shutdown(void);
    Light &ambientLight(void);
    void beginFrame(void);
    void endFrame(void);
    MatrixMode matrixMode(void) const;
    void setMatrixMode(MatrixMode matrixMode);
    void loadIdentity(void);
    void translate(const Eigen::Vector3f &r);
    void scale(const Eigen::Vector3f &r);
    void rotation(float angle, const Eigen::Vector3f &r);
    void add(Mesh &mesh, Material &material);
    void add(DirectionalLight &directionalLight);
    void add(PointLight &pointLight);
    void add(SpotLight &spotLight);

private:
    int m_width;
    int m_height;
    Light m_ambientLight;
    MatrixMode m_matrixMode;
    Eigen::Matrix4f m_matrix[MatrixModeMax];
    Material *m_material;
    Eigen::Vector3f m_translate;
    Eigen::Vector3f m_scale;
    Eigen::Quaternionf m_rotation;
};

VOLCANO_INLINE Light &Renderer::ambientLight(void)
{
    return m_ambientLight;
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRPHICS_RENDERER_HPP
