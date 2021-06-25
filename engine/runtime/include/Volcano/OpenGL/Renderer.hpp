//
//
#ifndef VOLCANO_OPENGL_RENDERER_HPP
#define VOLCANO_OPENGL_RENDERER_HPP

#include <QSize>
#include <QColor>
#include <QVector3D>
#include <QMatrix4x4>

#include <QOpenGLFramebufferObject>

#include <Volcano/OpenGL/Common.hpp>

VOLCANO_OPENGL_BEGIN

class Renderer {
public:
    Renderer(void);
    virtual ~Renderer(void);

public:
    bool init(int width, int height);
    const QSize &size(void);
    void setSize(const QSize &v);
    void clear(void);
    const QColor &clearColor(void) const;
    void setClearColor(const QColor &v);
    void beginFrame(void);
    void endFrame(void);

public:
    void loadIdenity(void);
    void translate(const QVector3D &v);
    void scale(const QVector3D &v);
    void rotate(GLfloat angle, const QVector3D &axis);
    // void addMesh(Mesh *mesh);
    // void bindMaterial(...);
    // void addDirectionalLight(...);
    // void addPointLight(...);
    // void addSpotLight(...);

private:
    Functions *m_funcs;
    QSize m_size;
    QColor m_clearColor;
    QOpenGLFramebufferObject *m_gBuffer;
    QVector3D m_translate;
    QVector3D m_scale;
    QQuaternion m_rotation;
};

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_RENDERER_HPP
