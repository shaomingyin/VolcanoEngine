//
//
#include <QVector2d>
#include <QScopedPointer>

#define DEBUG_DRAW_IMPLEMENTATION
#include <Volcano/Editor/DebugDrawRenderer.hpp>

VOLCANO_EDITOR_BEGIN

static const char *linePointVertShaderSrc =
    "#version 150\n"
    "in vec3 in_Position;\n"
    "in vec4 in_ColorPointSize;\n"
    "out vec4 v_Color;\n"
    "uniform mat4 u_MvpMatrix;\n"
    "void main() {\n"
    "    gl_Position  = u_MvpMatrix * vec4(in_Position, 1.0);\n"
    "    gl_PointSize = in_ColorPointSize.w;\n"
    "    v_Color      = vec4(in_ColorPointSize.xyz, 1.0);\n"
    "}\n";

static const char *linePointFragShaderSrc =
    "#version 150\n"
    "in  vec4 v_Color;\n"
    "out vec4 out_FragColor;\n"
    "void main() {\n"
    "    out_FragColor = v_Color;\n"
    "}\n";

static const char *textVertShaderSrc =
    "#version 150\n"
    "in vec2 in_Position;\n"
    "in vec2 in_TexCoords;\n"
    "in vec3 in_Color;\n"
    "uniform vec2 u_screenDimensions;\n"
    "out vec2 v_TexCoords;\n"
    "out vec4 v_Color;\n"
    "void main() {\n"
    "    // Map to normalized clip coordinates:\n"
    "    float x = ((2.0 * (in_Position.x - 0.5)) / u_screenDimensions.x) - 1.0;\n"
    "    float y = 1.0 - ((2.0 * (in_Position.y - 0.5)) / u_screenDimensions.y);\n"
    "    gl_Position = vec4(x, y, 0.0, 1.0);\n"
    "    v_TexCoords = in_TexCoords;\n"
    "    v_Color     = vec4(in_Color, 1.0);\n"
    "}\n";

static const char *textFragShaderSrc =
    "#version 150\n"
    "in vec2 v_TexCoords;\n"
    "in vec4 v_Color;\n"
    "uniform sampler2D u_glyphTexture;\n"
    "out vec4 out_FragColor;\n"
    "void main() {\n"
    "    out_FragColor = v_Color;\n"
    "    out_FragColor.a = texture(u_glyphTexture, v_TexCoords).r;\n"
    "}\n";

static inline GLuint handleToGL(dd::GlyphTextureHandle handle)
{
    const std::size_t temp = reinterpret_cast<std::size_t>(handle);
    return static_cast<GLuint>(temp);
}

static inline dd::GlyphTextureHandle GLToHandle(const GLuint id)
{
    const std::size_t temp = static_cast<std::size_t>(id);
    return reinterpret_cast<dd::GlyphTextureHandle>(temp);
}

DebugDrawRenderer::DebugDrawRenderer(QObject *parent):
    QObject(parent),
    m_linePointProgram(nullptr),
    m_linePointProgram_MvpMatrixLocation(-1),
    m_textProgram(nullptr),
    m_textProgram_GlyphTextureLocation(-1),
    m_textProgram_ScreenDimensions(-1),
    m_linePointVAO(nullptr),
    m_linePointVBO(nullptr),
    m_textVAO(nullptr),
    m_textVBO(nullptr)
{
}

DebugDrawRenderer::~DebugDrawRenderer(void)
{
}

bool DebugDrawRenderer::init(int width, int height)
{
    std::size_t offset = 0;

    m_width = width;
    m_height = height;

    m_mvpMatrix.setToIdentity();

    spdlog::debug("DebugDrawRenderer::setupShaderPrograms()\n");

    //
    // Line/point drawing shader
    //

    QScopedPointer<QOpenGLShaderProgram> linePointProgram(new QOpenGLShaderProgram(this));
    if (!linePointProgram || !linePointProgram->create()) {
        spdlog::error("Failed to create line point program for debug draw renderer.");
        return false;
    }

    if (!linePointProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, linePointVertShaderSrc)) {
        spdlog::error("Failed to init line point vertex shader for debug draw renderer.");
        return false;
    }

    if (!linePointProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, linePointFragShaderSrc)) {
        spdlog::error("Failed to init line point fragment shader for debug draw renderer.");
        return false;
    }

    linePointProgram->bindAttributeLocation("in_Position", 0);
    linePointProgram->bindAttributeLocation("in_ColoePointSize", 1);

    if (!linePointProgram->link()) {
        spdlog::error("Failed to link line point shader program for debug draw renderer.");
        return false;
    }

    m_linePointProgram_MvpMatrixLocation = linePointProgram->uniformLocation("u_MvpMatrix");
    if (m_linePointProgram_MvpMatrixLocation < 0) {
        spdlog::error("Unable to get u_MvpMatrix uniform location!");
        return false;
    }

    //
    // Text drawing shader.
    //

    QScopedPointer<QOpenGLShaderProgram> textProgram(new QOpenGLShaderProgram(this));
    if (!textProgram || !textProgram->create()) {
        spdlog::error("Failed to create text shader program for debug draw renderer.");
        return false;
    }

    if (!textProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, textVertShaderSrc)) {
        spdlog::error("Faield to init text vertex shader for debug draw renderer.");
        return false;
    }

    if (!textProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, textFragShaderSrc)) {
        spdlog::error("Failed to init text fragment shader for debug draw renderer.");
        return false;
    }

    textProgram->bindAttributeLocation("in_Position", 0);
    textProgram->bindAttributeLocation("in_TexCoords", 1);
    textProgram->bindAttributeLocation("in_Color", 2);

    if (!textProgram->link()) {
        spdlog::error("Failed to link text shader program for debug draw renderer.");
        return false;
    }

    m_textProgram_GlyphTextureLocation = textProgram->uniformLocation("u_glyphTexture");
    if (m_textProgram_GlyphTextureLocation < 0) {
        spdlog::error("Unable to get u_glyphTexture uniform location!");
        return false;
    }

    m_textProgram_ScreenDimensions = textProgram->uniformLocation("u_screenDimensions");
    if (m_textProgram_ScreenDimensions < 0) {
        spdlog::error("Unable to get u_screenDimensions uniform location!");
        return false;
    }

    std::printf("> DDRenderInterfaceCoreGL::setupVertexBuffers()\n");

    //
    // Lines/points vertex buffer:
    //

    QScopedPointer<QOpenGLVertexArrayObject> linePointVAO(new QOpenGLVertexArrayObject(this));
    if (!linePointVAO || !linePointVAO->create()) {
        spdlog::error("Failed to create line point vao for debug draw renderer.");
        return false;
    }

    QScopedPointer<QOpenGLBuffer> linePointVBO(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer));
    if (!linePointVBO || !linePointVBO->create()) {
        spdlog::error("Failed to create line point vbo for debug draw renderer.");
        return false;
    }

    linePointVAO->bind();

    linePointVBO->bind();
    linePointVBO->allocate(DEBUG_DRAW_VERTEX_BUFFER_SIZE * sizeof(dd::DrawVertex));

    offset = 0;

    glEnableVertexAttribArray(0); // in_Position (vec3)
    glVertexAttribPointer(
            /* index     = */ 0,
            /* size      = */ 3,
            /* type      = */ GL_FLOAT,
            /* normalize = */ GL_FALSE,
            /* stride    = */ sizeof(dd::DrawVertex),
            /* offset    = */ reinterpret_cast<void *>(offset));
    offset += sizeof(float) * 3;

    glEnableVertexAttribArray(1); // in_ColorPointSize (vec4)
    glVertexAttribPointer(
            /* index     = */ 1,
            /* size      = */ 4,
            /* type      = */ GL_FLOAT,
            /* normalize = */ GL_FALSE,
            /* stride    = */ sizeof(dd::DrawVertex),
            /* offset    = */ reinterpret_cast<void *>(offset));

    linePointVBO->release();
    linePointVAO->release();

    //
    // Text rendering vertex buffer:
    //

    QScopedPointer<QOpenGLVertexArrayObject> textVAO(new QOpenGLVertexArrayObject(this));
    if (!textVAO || !textVAO->create()) {
        spdlog::error("Failed to create text vao for debug draw renderer.");
        return false;
    }

    QScopedPointer<QOpenGLBuffer> textVBO(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer));
    if (!textVBO || !textVBO->create()) {
        spdlog::error("Failed to create text vbo for debug draw renderer.");
        return false;
    }

    textVAO->bind();

    textVBO->bind();
    textVBO->allocate(DEBUG_DRAW_VERTEX_BUFFER_SIZE * sizeof(dd::DrawVertex));

    offset = 0;

    glEnableVertexAttribArray(0); // in_Position (vec2)
    glVertexAttribPointer(
            /* index     = */ 0,
            /* size      = */ 2,
            /* type      = */ GL_FLOAT,
            /* normalize = */ GL_FALSE,
            /* stride    = */ sizeof(dd::DrawVertex),
            /* offset    = */ reinterpret_cast<void *>(offset));
    offset += sizeof(float) * 2;

    glEnableVertexAttribArray(1); // in_TexCoords (vec2)
    glVertexAttribPointer(
            /* index     = */ 1,
            /* size      = */ 2,
            /* type      = */ GL_FLOAT,
            /* normalize = */ GL_FALSE,
            /* stride    = */ sizeof(dd::DrawVertex),
            /* offset    = */ reinterpret_cast<void *>(offset));
    offset += sizeof(float) * 2;

    glEnableVertexAttribArray(2); // in_Color (vec4)
    glVertexAttribPointer(
            /* index     = */ 2,
            /* size      = */ 4,
            /* type      = */ GL_FLOAT,
            /* normalize = */ GL_FALSE,
            /* stride    = */ sizeof(dd::DrawVertex),
            /* offset    = */ reinterpret_cast<void *>(offset));

    textVBO->release();
    textVAO->release();

    m_linePointProgram = linePointProgram.take();
    m_linePointVAO = linePointVAO.take();
    m_linePointVBO = linePointVBO.take();
    m_textProgram = textProgram.take();
    m_textVAO = textVAO.take();
    m_textVBO = textVBO.take();

    return true;
}

void DebugDrawRenderer::resize(int width, int height)
{
    m_width = width;
    m_height = height;
}

void DebugDrawRenderer::beginDraw(void)
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glDisable(GL_BLEND);
}

void DebugDrawRenderer::endDraw(void)
{
}

dd::GlyphTextureHandle DebugDrawRenderer::createGlyphTexture(int width, int height, const void *pixels)
{
    Q_ASSERT(width > 0 && height > 0);
    Q_ASSERT(pixels != nullptr);

    GLuint textureId = 0;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, pixels);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, 0);

    return GLToHandle(textureId);
}

void DebugDrawRenderer::destroyGlyphTexture(dd::GlyphTextureHandle glyphTex)
{
    if (glyphTex != nullptr) {
        const GLuint textureId = handleToGL(glyphTex);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDeleteTextures(1, &textureId);
    }
}

void DebugDrawRenderer::drawPointList(const dd::DrawVertex *points, int count, bool depthEnabled)
{
    Q_ASSERT(points != nullptr);
    Q_ASSERT(count > 0 && count <= DEBUG_DRAW_VERTEX_BUFFER_SIZE);

    m_linePointVAO->bind();
    m_linePointProgram->bind();
    m_linePointProgram->setUniformValue(m_linePointProgram_MvpMatrixLocation, m_mvpMatrix);

    if (depthEnabled)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    m_linePointVBO->bind();
    m_linePointVBO->write(0, points, count * sizeof(dd::DrawVertex));

    glDrawArrays(GL_POINTS, 0, count);
}

void DebugDrawRenderer::drawLineList(const dd::DrawVertex *lines, int count, bool depthEnabled)
{
    Q_ASSERT(lines != nullptr);
    Q_ASSERT(count > 0 && count <= DEBUG_DRAW_VERTEX_BUFFER_SIZE);

    m_linePointVAO->bind();
    m_linePointProgram->bind();
    m_linePointProgram->setUniformValue(m_linePointProgram_MvpMatrixLocation, m_mvpMatrix);

    if (depthEnabled)
        glEnable(GL_DEPTH_TEST);
    else
        glDisable(GL_DEPTH_TEST);

    m_linePointVBO->bind();
    m_linePointVBO->write(0, lines, count * sizeof(dd::DrawVertex));

    glDrawArrays(GL_LINES, 0, count);
}

void DebugDrawRenderer::drawGlyphList(const dd::DrawVertex *glyphs, int count, dd::GlyphTextureHandle glyphTex)
{
    Q_ASSERT(glyphs != nullptr);
    Q_ASSERT(count > 0 && count <= DEBUG_DRAW_VERTEX_BUFFER_SIZE);

    m_textVAO->bind();
    m_textProgram->bind();
    m_textProgram->setUniformValue(m_textProgram_GlyphTextureLocation, 0);
    m_textProgram->setUniformValue(m_textProgram_ScreenDimensions, QVector2D(m_width, m_height));

    if (glyphTex != nullptr) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, handleToGL(glyphTex));
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_DEPTH_TEST);

    m_textVBO->bind();
    m_textVBO->write(0, glyphs, count * sizeof(dd::DrawVertex));

    glDrawArrays(GL_TRIANGLES, 0, count);
}

VOLCANO_EDITOR_END
