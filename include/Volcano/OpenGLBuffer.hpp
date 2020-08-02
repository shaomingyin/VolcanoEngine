//
//
#ifndef VOLCANO_OPENGLBUFFER_HPP
#define VOLCANO_OPENGLBUFFER_HPP

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class VOLCANO_API OpenGLBuffer
{
public:
    OpenGLBuffer(void);
    virtual ~OpenGLBuffer(void);

public:
    virtual int size(void) = 0;
};

VOLCANO_END

#endif // VOLCANO_OPENGLBUFFER_HPP
