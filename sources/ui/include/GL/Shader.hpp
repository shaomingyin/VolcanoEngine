//
//
#ifndef GL_SHADER_HPP
#define GL_SHADER_HPP

#include <string>

#include <GL/Common.hpp>
#include <GL/Object.hpp>

GL_BEGIN

class GL_API Shader: public Object {
public:
    Shader(GLenum type = GL_INVALID_ENUM);
    ~Shader(void) override;

public:
    bool create(void);
    void destroy(void);
    GLenum type(void) const;
    void setType(GLenum v);
    void setSource(const char *p);
    void setSource(const std::string &v);
    bool compile(void);
    bool isCompiled(void) const;
    const std::string &infoLog(void) const;

private:
    GLenum m_type;
    bool m_compiled;
    std::string m_infoLog;
};

GL_INLINE GLenum Shader::type(void) const
{
    return m_type;
}

GL_INLINE void Shader::setType(GLenum v)
{
    GL_ASSERT(!isValid());
    m_type = v;
}

GL_INLINE void Shader::setSource(const char *p)
{
    GL_ASSERT(isValid());
    GL_ASSERT(p != nullptr);

    vglShaderSource(m_id, 1, &p, nullptr);
}

GL_INLINE void Shader::setSource(const std::string &v)
{
    GL_ASSERT(isValid());

    setSource(v.c_str());
}

GL_INLINE bool Shader::isCompiled(void) const
{
    return (isValid() && m_compiled);
}

GL_INLINE const std::string &Shader::infoLog(void) const
{
    GL_ASSERT(isValid());

    return m_infoLog;
}

GL_END

#endif // GL_SHADER_HPP
