//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_HPP
#define VOLCANO_GRAPHICS_RENDERER_HPP

#include <QVector3D>

#include <Volcano/Graphics/Common.hpp>

VOLCANO_GRAPHICS_BEGIN

class VOLCANO_API Renderer
{
public:
    Renderer(void);
    virtual ~Renderer(void);

public:
    bool init(void);
    void reset(int width, int height);
    void render(void);
    void lookAt(const QVector3D &pos, const QVector3D &dir, const QVector3D &up);

private:
    OpenGLFunctions *m_gl;
    int m_width;
    int m_height;
    QVector3D m_viewPos;
    QVector3D m_viewDir;
    QVector3D m_viewUp;
};

VOLCANO_INLINE void Renderer::lookAt(const QVector3D &pos, const QVector3D &dir, const QVector3D &up)
{
    m_viewPos = pos;
    m_viewDir = dir.normalized();
    m_viewUp = up.normalized();
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_HPP
