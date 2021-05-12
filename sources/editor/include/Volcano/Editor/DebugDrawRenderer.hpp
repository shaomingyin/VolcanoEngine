//
//
#ifndef VOLCANO_EDITOR_DEBUGDRAWRENDERER_HPP
#define VOLCANO_EDITOR_DEBUGDRAWRENDERER_HPP

#define DEBUG_DRAW_EXPLICIT_CONTEXT
#include <debug_draw.hpp>

#include <GL/gl3w.h>

#include <QObject>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include <Volcano/Editor/Common.hpp>

VOLCANO_EDITOR_BEGIN

class DebugDrawRenderer: public QObject, public dd::RenderInterface {
    Q_OBJECT

public:
    DebugDrawRenderer(QObject *parent = nullptr);
    ~DebugDrawRenderer(void) override;

public:
    bool init(int width, int height);
    void resize(int width, int height);
    void beginDraw(void) override;
    void endDraw(void) override;
    dd::GlyphTextureHandle createGlyphTexture(int width, int height, const void *pixels) override;
    void destroyGlyphTexture(dd::GlyphTextureHandle glyphTex) override;
    void drawPointList(const dd::DrawVertex *points, int count, bool depthEnabled) override;
    void drawLineList(const dd::DrawVertex *lines, int count, bool depthEnabled) override;
    void drawGlyphList(const dd::DrawVertex *glyphs, int count, dd::GlyphTextureHandle glyphTex) override;

private:
    int m_width;
    int m_height;
    QMatrix4x4 m_mvpMatrix;
    QOpenGLShaderProgram *m_linePointProgram;
    GLint  m_linePointProgram_MvpMatrixLocation;
    QOpenGLShaderProgram *m_textProgram;
    GLint  m_textProgram_GlyphTextureLocation;
    GLint  m_textProgram_ScreenDimensions;
    QOpenGLVertexArrayObject *m_linePointVAO;
    QOpenGLBuffer *m_linePointVBO;
    QOpenGLVertexArrayObject *m_textVAO;
    QOpenGLBuffer *m_textVBO;
};

VOLCANO_EDITOR_END

#endif // VOLCANO_EDITOR_DEBUGDRAWRENDERER_HPP
