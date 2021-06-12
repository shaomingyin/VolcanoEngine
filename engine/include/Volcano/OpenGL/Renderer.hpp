//
//
#ifndef VOLCANO_OPENGL_RENDERER_HPP
#define VOLCANO_OPENGL_RENDERER_HPP

#include <QObject>
#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>

#include <Volcano/OpenGL/Common.hpp>
#include <Volcano/OpenGL/Mesh.hpp>
#include <Volcano/OpenGL/Material.hpp>
#include <Volcano/OpenGL/Light.hpp>
#include <Volcano/OpenGL/DirectionalLight.hpp>
#include <Volcano/OpenGL/PointLight.hpp>
#include <Volcano/OpenGL/SpotLight.hpp>

VOLCANO_OPENGL_BEGIN

class Renderer: public QObject {
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
    bool init(int width, int height);
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
    Functions *m_functions;
    bool m_clearEnabled;
    QVector3D m_clearColor;
    QVector4D m_viewport;
    int m_width;
    int m_height;
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

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_RENDERER_HPP
