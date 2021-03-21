//
//
#ifndef GL_PROGRAM_HPP
#define GL_PROGRAM_HPP

#include <string>

#include <GL/Common.hpp>
#include <GL/Shader.hpp>
#include <GL/Object.hpp>

GL_BEGIN

class GL_API Program: public Object {
public:
    Program(void);
    ~Program(void) override;

public:
    bool create(void);
    void destroy(void);
    bool attachShader(const Shader &r);
    bool attachShader(GLenum shaderType, const char *shaderSource);
    bool attachShader(GLenum shaderType, const std::string &shaderSource);
    bool link(void);
    bool isLinked(void) const;
    const std::string &infoLog(void) const;
    void use(void);
    bool isUsing(void) const;

public:
    int uniformLocation(const char *name);
    void setUniform(int loc, GLint v);
    void setUniform(const char *name, GLint v);
    void setUniform(int loc, GLfloat v);
    void setUniform(const char *name, GLfloat v);
    void setUniform(int loc, GLdouble v);
    void setUniform(const char *name, GLdouble v);
    void setUniform(int loc, const Eigen::Vector2i &r);
    void setUniform(const char *name, const Eigen::Vector2i &r);
    void setUniform(int loc, const Eigen::Vector3i &r);
    void setUniform(const char *name, const Eigen::Vector3i &r);
    void setUniform(int loc, const Eigen::Vector2f &r);
    void setUniform(const char *name, const Eigen::Vector2f &r);
    void setUniform(int loc, const Eigen::Vector3f &r);
    void setUniform(const char *name, const Eigen::Vector3f &r);
    void setUniform(int loc, const Eigen::Matrix2f &r);
    void setUniform(const char *name, const Eigen::Matrix2f &r);
    void setUniform(int loc, const Eigen::Matrix3f &r);
    void setUniform(const char *name, const Eigen::Matrix3f &r);
    void setUniform(int loc, const Eigen::Matrix4f &r);
    void setUniform(const char *name, const Eigen::Matrix4f &r);

private:
    bool m_linked;
    std::string m_infoLog;
};

GL_INLINE bool Program::attachShader(const Shader &r)
{
    GL_ASSERT(isValid());
    GL_ASSERT(!isLinked());

    glAttachShader(m_id, r.id());

    return true;
}

GL_INLINE bool Program::attachShader(GLenum shaderType, const std::string &shaderSource)
{
    GL_ASSERT(isValid());
    GL_ASSERT(!isLinked());

    return attachShader(shaderType, shaderSource.c_str());
}

GL_INLINE bool Program::isLinked(void) const
{
    return (isValid() && m_linked);
}

GL_INLINE const std::string &Program::infoLog(void) const
{
    GL_ASSERT(isValid());

    return m_infoLog;
}

GL_INLINE int Program::uniformLocation(const char *name)
{
    GL_ASSERT(isValid());
    GL_ASSERT(name != nullptr);

    return glGetUniformLocation(m_id, name);
}

GL_INLINE void Program::setUniform(int loc, GLint v)
{
    GL_ASSERT(isUsing());
    GL_ASSERT(loc >= 0);

    glUniform1i(loc, v);
}

GL_INLINE void Program::setUniform(const char *name, GLint v)
{
    GL_ASSERT(isUsing());
    GL_ASSERT(name != nullptr);

    glUniform1i(uniformLocation(name), v);
}

GL_INLINE void Program::setUniform(int loc, GLfloat v)
{
    GL_ASSERT(isUsing());
    GL_ASSERT(loc >= 0);

    glUniform1f(loc, v);
}

GL_INLINE void Program::setUniform(const char *name, GLfloat v)
{
    GL_ASSERT(isUsing());
    GL_ASSERT(name != nullptr);

    glUniform1f(uniformLocation(name), v);
}

GL_INLINE void Program::setUniform(int loc, GLdouble v)
{
    GL_ASSERT(isUsing());
    GL_ASSERT(loc >= 0);

    glUniform1d(loc, v);
}

GL_INLINE void Program::setUniform(const char *name, GLdouble v)
{
    GL_ASSERT(isUsing());
    GL_ASSERT(name != nullptr);

    glUniform1d(uniformLocation(name), v);
}

GL_INLINE void Program::setUniform(int loc, const Eigen::Vector2i &r)
{
    GL_ASSERT(isUsing());
    GL_ASSERT(loc >= 0);

    glUniform2i(loc, r[0], r[1]);
}

GL_INLINE void Program::setUniform(const char *name, const Eigen::Vector2i &r)
{
    GL_ASSERT(isUsing());
    GL_ASSERT(name != nullptr);

    glUniform2i(uniformLocation(name), r[0], r[1]);
}

GL_INLINE void Program::setUniform(int loc, const Eigen::Vector3i &r)
{
    GL_ASSERT(isUsing());
    GL_ASSERT(loc >= 0);

    glUniform3i(loc, r[0], r[1], r[2]);
}

GL_INLINE void Program::setUniform(const char *name, const Eigen::Vector3i &r)
{
    GL_ASSERT(isUsing());
    GL_ASSERT(name != nullptr);

    glUniform3i(uniformLocation(name), r[0], r[1], r[2]);
}

GL_INLINE void Program::setUniform(int loc, const Eigen::Vector2f &r)
{
    GL_ASSERT(isUsing());
    GL_ASSERT(loc >= 0);

    glUniform2f(loc, r[0], r[1]);
}

GL_INLINE void Program::setUniform(const char *name, const Eigen::Vector2f &r)
{
    GL_ASSERT(isUsing());
    GL_ASSERT(name != nullptr);

    glUniform2f(uniformLocation(name), r[0], r[1]);
}

GL_INLINE void Program::setUniform(int loc, const Eigen::Vector3f &r)
{
    GL_ASSERT(isUsing());
    GL_ASSERT(loc >= 0);

    glUniform3f(loc, r[0], r[1], r[2]);
}

GL_INLINE void Program::setUniform(const char *name, const Eigen::Vector3f &r)
{
    GL_ASSERT(isUsing());
    GL_ASSERT(name != nullptr);

    glUniform3f(uniformLocation(name), r[0], r[1], r[2]);
}

GL_INLINE void Program::setUniform(int loc, const Eigen::Matrix2f &r)
{
    GL_ASSERT(isUsing());
    GL_ASSERT(loc >= 0);

    glUniformMatrix2fv(loc, 1, GL_FALSE, r.data());
}

GL_INLINE void Program::setUniform(const char *name, const Eigen::Matrix2f &r)
{
    GL_ASSERT(isUsing());
    GL_ASSERT(name != nullptr);

    glUniformMatrix2fv(uniformLocation(name), 1, GL_FALSE, r.data());
}

GL_INLINE void Program::setUniform(int loc, const Eigen::Matrix3f &r)
{
    GL_ASSERT(isUsing());
    GL_ASSERT(loc >= 0);

    glUniformMatrix3fv(loc, 1, GL_FALSE, r.data());
}

GL_INLINE void Program::setUniform(const char *name, const Eigen::Matrix3f &r)
{
    GL_ASSERT(isUsing());
    GL_ASSERT(name != nullptr);

    glUniformMatrix3fv(uniformLocation(name), 1, GL_FALSE, r.data());
}

GL_INLINE void Program::setUniform(int loc, const Eigen::Matrix4f &r)
{
    GL_ASSERT(isUsing());
    GL_ASSERT(loc >= 0);

    glUniformMatrix4fv(loc, 1, GL_FALSE, r.data());
}

GL_INLINE void Program::setUniform(const char *name, const Eigen::Matrix4f &r)
{
    GL_ASSERT(isUsing());
    GL_ASSERT(name != nullptr);

    glUniformMatrix4fv(uniformLocation(name), 1, GL_FALSE, r.data());
}

GL_INLINE void Program::use(void)
{
    GL_ASSERT(isLinked());

    glUseProgram(m_id);
}

GL_INLINE bool Program::isUsing(void) const
{
    GL_ASSERT(isLinked());

    GLint id;
    glGetIntegerv(GL_CURRENT_PROGRAM, &id);

    return id == GLint(m_id);
}

GL_END

#endif // GL_PROGRAM_HPP
