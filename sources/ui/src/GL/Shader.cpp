//
//
#include <GL/Shader.hpp>

GL_BEGIN

Shader::Shader(GLenum type):
    m_type(type),
    m_compiled(false)
{
}

Shader::~Shader(void)
{
    if (isValid())
        destroy();
}

bool Shader::create(void)
{
    GL_ASSERT(!isValid());
    GL_ASSERT(m_type != GL_INVALID_ENUM);

    m_id = vglCreateShader(m_type);
    if (m_id < 1)
        return false;

    m_compiled = false;
    m_infoLog.clear();

    return true;
}

void Shader::destroy(void)
{
    GL_ASSERT(isValid());

    vglDeleteShader(m_id);
    m_id = 0;
    m_compiled = false;
    m_infoLog.clear();
}

bool Shader::compile(void)
{
    GL_ASSERT(isValid());
    GL_ASSERT(!isCompiled());

    vglCompileShader(m_id);

    GLint result;
    vglGetShaderiv(m_id, GL_COMPILE_STATUS, &result);
    if (result != GL_TRUE) {
        char infoLog[256] = { 0 };
        GLsizei length;
        vglGetShaderInfoLog(m_id, sizeof(infoLog), &length, infoLog);
        m_infoLog = infoLog;
        return false;
    }

    m_compiled = true;

    return true;
}

GL_END
