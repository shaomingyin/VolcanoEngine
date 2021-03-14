//
//
#include <GL/Buffer.hpp>

GL_BEGIN

Buffer::Buffer(GLenum target, GLenum usage):
    m_target(target),
    m_usage(usage)
{
}

Buffer::~Buffer(void)
{
    GL_ASSERT(!isValid());
}

bool Buffer::create(GLsizeiptr size, const void *data)
{
    GL_ASSERT(!isValid());
    GL_ASSERT(m_target != GL_INVALID_ENUM);
    GL_ASSERT(m_usage != GL_INVALID_ENUM);
    GL_ASSERT(size > 0);

    vglGenBuffers(1, &m_id);
    if (m_id < 1)
        return false;

    vglBindBuffer(m_target, m_id);
    vglBufferData(m_target, size, data, m_usage);
    m_size = size;

    return true;
}

void Buffer::destroy(void)
{
    GL_ASSERT(isValid());

    vglDeleteBuffers(1, &m_id);
    m_id = 0;
}

GLenum Buffer::targetToBinding(GLenum target)
{
    GLenum binding;

    switch (target) {
    case GL_ARRAY_BUFFER:
        binding = GL_ARRAY_BUFFER_BINDING;
        break;
    case GL_ATOMIC_COUNTER_BUFFER:
        binding = GL_ATOMIC_COUNTER_BUFFER_BINDING;
        break;
    case GL_COPY_READ_BUFFER:
        binding = GL_COPY_READ_BUFFER_BINDING;
        break;
    case GL_COPY_WRITE_BUFFER:
        binding = GL_COPY_WRITE_BUFFER_BINDING;
        break;
    case GL_DRAW_INDIRECT_BUFFER:
        binding = GL_DRAW_INDIRECT_BUFFER_BINDING;
        break;
    case GL_DISPATCH_INDIRECT_BUFFER:
        binding = GL_DISPATCH_INDIRECT_BUFFER_BINDING;
        break;
    case GL_ELEMENT_ARRAY_BUFFER:
        binding = GL_ELEMENT_ARRAY_BUFFER_BINDING;
        break;
    case GL_PIXEL_PACK_BUFFER:
        binding = GL_PIXEL_PACK_BUFFER_BINDING;
        break;
    case GL_PIXEL_UNPACK_BUFFER:
        binding = GL_PIXEL_UNPACK_BUFFER_BINDING;
        break;
    case GL_SHADER_STORAGE_BUFFER:
        binding = GL_SHADER_STORAGE_BUFFER_BINDING;
        break;
    case GL_TEXTURE_BUFFER:
        binding = GL_TEXTURE_BUFFER_BINDING;
        break;
    case GL_TRANSFORM_FEEDBACK_BUFFER:
        binding = GL_TRANSFORM_FEEDBACK_BUFFER_BINDING;
        break;
    case GL_UNIFORM_BUFFER:
        binding = GL_UNIFORM_BUFFER_BINDING;
        break;
    default:
        binding = GL_INVALID_ENUM;
        break;
    }

    return binding;
}

GL_END
