//
//
#ifndef GL_BUFFER_HPP
#define GL_BUFFER_HPP

#include <GL/Common.hpp>
#include <GL/Object.hpp>

GL_BEGIN

class GL_API Buffer: public Object {
public:
    Buffer(GLenum target = GL_INVALID_ENUM, GLenum usage = GL_INVALID_ENUM);
    ~Buffer(void) override;

public:
    bool create(GLsizeiptr size, const void *data = nullptr);
    void destroy(void);
    GLenum target(void) const;
    void setTarget(GLenum v);
    GLenum usage(void) const;
    void setUsage(GLenum v);
    GLsizeiptr size(void) const;
    void bind(void);
    bool isBound(void) const;
    void setData(GLsizeiptr length, const void *data = nullptr);
    void setSubData(GLintptr offset, GLsizeiptr length, const void *data);
    void *map(GLenum access = GL_READ_WRITE);
    void *mapRange(GLintptr offset, GLsizeiptr length, GLbitfield access = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT);
    void unmap(void);
    static GLenum targetToBinding(GLenum target);

private:
    GLenum m_target;
    GLenum m_usage;
    GLsizeiptr m_size;
};

GL_INLINE GLenum Buffer::target(void) const
{
    return m_target;
}

GL_INLINE void Buffer::setTarget(GLenum v)
{
    GL_ASSERT(!isValid());

    m_target = v;
}

GL_INLINE GLenum Buffer::usage(void) const
{
    return m_usage;
}

GL_INLINE void Buffer::setUsage(GLenum v)
{
    GL_ASSERT(!isValid());

    m_usage = v;
}

GL_INLINE GLsizeiptr Buffer::size(void) const
{
    GL_ASSERT(isValid());

    return m_size;
}

GL_INLINE void Buffer::bind(void)
{
    GL_ASSERT(isValid());

    vglBindBuffer(m_target, m_id);
}

GL_INLINE bool Buffer::isBound(void) const
{
    GL_ASSERT(isValid());

    GLint id = -1;
    vglGetIntegerv(targetToBinding(m_target), &id);

    return id == GLint(m_id);
}

GL_INLINE void Buffer::setData(GLsizeiptr length, const void *data)
{
    GL_ASSERT(isBound());
    GL_ASSERT(0 < length && length <= m_size);

    vglBufferData(m_target, length, data, m_usage);
}

GL_INLINE void Buffer::setSubData(GLintptr offset, GLsizeiptr length, const void *data)
{
    GL_ASSERT(isBound());
    GL_ASSERT(0 <= offset && offset < m_size);
    GL_ASSERT(length > 0);
    GL_ASSERT((offset + length) < m_size);

    vglBufferSubData(m_target, offset, length, data);
}

GL_INLINE void *Buffer::map(GLenum access)
{
    GL_ASSERT(isBound());

    return vglMapBuffer(m_target, access);
}

GL_INLINE void *Buffer::mapRange(GLintptr offset, GLsizeiptr length, GLbitfield access)
{
    GL_ASSERT(isBound());
    GL_ASSERT(0 <= offset && offset < m_size);
    GL_ASSERT(length > 0);
    GL_ASSERT((offset + length) < m_size);

    return vglMapBufferRange(m_target, offset, length, access);
}

GL_INLINE void Buffer::unmap(void)
{
    GL_ASSERT(isBound());

    vglUnmapBuffer(m_id);
}

GL_END

#endif // GL_BUFFER_HPP
