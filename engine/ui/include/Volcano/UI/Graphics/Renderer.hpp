//
//
#ifndef VOLCANO_UI_GRPHICS_RENDERER_HPP
#define VOLCANO_UI_GRPHICS_RENDERER_HPP

#include <QObject>
#include <QSurface>
#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

#include <Volcano/UI/Graphics/Common.hpp>
#include <Volcano/UI/Graphics/Mesh.hpp>
#include <Volcano/UI/Graphics/Material.hpp>
#include <Volcano/UI/Graphics/Light.hpp>
#include <Volcano/UI/Graphics/DirectionalLight.hpp>
#include <Volcano/UI/Graphics/PointLight.hpp>
#include <Volcano/UI/Graphics/SpotLight.hpp>

VOLCANO_UI_GRAPHICS_BEGIN

class VOLCANO_UI_API Renderer: public QObject {
    Q_OBJECT

public:
    enum MatrixMode {
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
    void add(Mesh &mesh, Material &material);
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

VOLCANO_UI_GRAPHICS_END

#endif // VOLCANO_UI_GRPHICS_RENDERER_HPP
