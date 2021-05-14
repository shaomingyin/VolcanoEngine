//
//
#ifndef GL_OBJECT_HPP
#define GL_OBJECT_HPP

#include <GL/Common.hpp>

GL_BEGIN

class GL_API Object {
public:
    Object(void);
    Object(const Object &that) = delete;
    virtual ~Object(void);

public:
    GLuint id(void) const;
    bool isValid(void) const;
    Object &operator=(const Object &that) = delete;

protected:
    GLuint m_id;
};

GL_INLINE GLuint Object::id(void) const
{
    return m_id;
}

GL_INLINE bool Object::isValid(void) const
{
    return m_id > 0;
}

GL_END

#endif // GL_OBJECT_HPP
