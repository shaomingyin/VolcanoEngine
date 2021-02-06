//
//
#ifndef VOLCANO_GRPHICS_RENDERER_HPP
#define VOLCANO_GRPHICS_RENDERER_HPP

#include <QObject>
#include <QSurface>
#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Mesh.hpp>
#include <Volcano/Graphics/Material.hpp>
#include <Volcano/Graphics/Light.hpp>
#include <Volcano/Graphics/DirectionalLight.hpp>
#include <Volcano/Graphics/PointLight.hpp>
#include <Volcano/Graphics/SpotLight.hpp>

VOLCANO_GRAPHICS_BEGIN

class VOLCANO_API Renderer: public QObject
{
    Q_OBJECT

public:
    enum MatrixMode
    {
        MatrixModeModel = 0,
        MatrixModeView,
        MatrixModeProjection,
        MatrixModeMax
    };

public:
    Renderer(QObject *parent = nullptr);
    ~Renderer(void) override;

public:
    bool init(QSurface *surface);
    void shutdown(void);
    Light &ambientLight(void);
    void beginFrame(void);
    void endFrame(void);
    MatrixMode matrixMode(void) const;
    void setMatrixMode(MatrixMode matrixMode);
    void loadIdentity(void);
    void translate(const QVector3D &r);
    void scale(const QVector3D &r);
    void rotation(float angle, const QVector3D &r);
    void bind(Material &material);
    void add(Mesh &mesh);
    void add(DirectionalLight &directionalLight);
    void add(PointLight &pointLight);
    void add(SpotLight &spotLight);

private:
    QSurface *m_surface;
    OpenGLFunctions *m_gl;
    Light m_ambientLight;
    MatrixMode m_matrixMode;
    QMatrix4x4 m_matrix[MatrixModeMax];
    Material *m_material;
    QVector3D m_translate;
    QVector3D m_scale;
    QQuaternion m_rotation;
};

VOLCANO_INLINE Light &Renderer::ambientLight(void)
{
    return m_ambientLight;
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRPHICS_RENDERER_HPP
