//
//
#include <GL/Program.hpp>

GL_BEGIN

Program::Program(void):
    m_linked(false)
{
}

Program::~Program(void)
{
    if (isValid())
        destroy();
}

bool Program::create(void)
{
    GL_ASSERT(!isValid());

    m_id = glCreateProgram();
    if (m_id < 1)
        return false;

    m_infoLog.clear();

    return true;
}

void Program::destroy(void)
{
    GL_ASSERT(isValid());

    glDeleteProgram(m_id);
    m_id = 0;
    m_linked = false;
    m_infoLog.clear();
}

bool Program::attachShader(GLenum shaderType, const char *shaderSource)
{
    GL_ASSERT(isValid());
    GL_ASSERT(!isLinked());
    GL_ASSERT(shaderSource != nullptr);

    Shader shader(shaderType);
    if (!shader.create())
        return false;

    shader.setSource(shaderSource);
    if (!shader.compile())
        return false;

    return attachShader(shader);
}

bool Program::link(void)
{
    GL_ASSERT(isValid());
    GL_ASSERT(!isLinked());

    glLinkProgram(m_id);

    GLint result;
    glGetProgramiv(m_id, GL_LINK_STATUS, &result);
    if (result != GL_TRUE) {
        char infoLog[256] = { 0 };
        GLsizei length;
        glGetProgramInfoLog(m_id, sizeof(infoLog), &length, infoLog);
        m_infoLog = infoLog;
        return false;
    }

    m_linked = true;

    return true;
}

GL_END
