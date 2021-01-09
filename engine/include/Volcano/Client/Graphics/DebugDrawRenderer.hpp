//
//
#ifndef VOLCANO_CLIENT_GRAPHICS_DEBUGDRAWRENDERER_HPP
#define VOLCANO_CLIENT_GRAPHICS_DEBUGDRAWRENDERER_HPP

#include <debug_draw.hpp>

#include <Volcano/Size.hpp>
#include <Volcano/Client/Graphics/Common.hpp>

VOLCANO_CLIENT_GRAPHICS_BEGIN

class DebugDrawRenderer final: public dd::RenderInterface {
public:
    DebugDrawRenderer(void);
    ~DebugDrawRenderer(void);

public:
    bool init(void);
    void shutdown(void);
    void drawPointList(const dd::DrawVertex * points, int count, bool depthEnabled) override;
    void drawLineList(const dd::DrawVertex * lines, int count, bool depthEnabled) override;
    void drawGlyphList(const dd::DrawVertex * glyphs, int count, dd::GlyphTextureHandle glyphTex) override;
    dd::GlyphTextureHandle createGlyphTexture(int width, int height, const void * pixels) override;
    void destroyGlyphTexture(dd::GlyphTextureHandle glyphTex) override;
    const SizeI &size(void) const;
    void resize(const SizeI &size);

private:
    void setupShaderPrograms(void);
    void setupVertexBuffers(void);
    static GLuint handleToGL(dd::GlyphTextureHandle handle);
    static dd::GlyphTextureHandle GLToHandle(const GLuint id);
    static void checkGLError(const char * file, const int line);
    static void compileShader(const GLuint shader);
    static void linkProgram(const GLuint program);

private:
    SizeI m_size;
    glm::mat4 m_mvpMatrix;

    GLuint m_linePointProgram;
    GLint  m_linePointProgram_MvpMatrixLocation;

    GLuint m_textProgram;
    GLint  m_textProgram_GlyphTextureLocation;
    GLint  m_textProgram_ScreenDimensions;

    GLuint m_linePointVAO;
    GLuint m_linePointVBO;

    GLuint m_textVAO;
    GLuint m_textVBO;

    static const char *c_linePointVertShaderSrc;
    static const char *c_linePointFragShaderSrc;

    static const char *c_textVertShaderSrc;
    static const char *c_textFragShaderSrc;
};

VOLCANO_INLINE const SizeI &DebugDrawRenderer::size(void) const
{
    return m_size;
}

VOLCANO_INLINE void DebugDrawRenderer::resize(const SizeI &size)
{
    m_size = size;
}

VOLCANO_INLINE GLuint DebugDrawRenderer::handleToGL(dd::GlyphTextureHandle handle)
{
    const std::size_t temp = reinterpret_cast<std::size_t>(handle);
    return static_cast<GLuint>(temp);
}

VOLCANO_INLINE dd::GlyphTextureHandle DebugDrawRenderer::GLToHandle(const GLuint id)
{
    const std::size_t temp = static_cast<std::size_t>(id);
    return reinterpret_cast<dd::GlyphTextureHandle>(temp);
}

VOLCANO_INLINE void DebugDrawRenderer::checkGLError(const char * file, const int line)
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        // VOLCANO_LOGE("%s(%d) : GL_CORE_ERROR=0x%X", file, line, err);
    }
}

VOLCANO_CLIENT_GRAPHICS_END

#endif // VOLCANO_CLIENT_GRAPHICS_DebugDrawRenderer_HPP
