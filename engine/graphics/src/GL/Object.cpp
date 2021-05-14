//
//
#include <GL/Object.hpp>

GL_BEGIN

Object::Object(void):
    m_id(0)
{
}

Object::~Object(void)
{
    GL_ASSERT(!isValid());
}

GL_END
