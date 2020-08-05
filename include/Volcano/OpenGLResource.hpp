//
//
#ifndef VOLCANO_OPENGLRESOURCE_HPP
#define VOLCANO_OPENGLRESOURCE_HPP

#include <Volcano/Common.hpp>
#include <Volcano/Resource.hpp>

VOLCANO_BEGIN

class VOLCANO_API OpenGLResource: public Resource
{
    Q_OBJECT

public:
    OpenGLResource(Node *node = nullptr);
    ~OpenGLResource(void) override;

public:
    virtual bool initGL(void) = 0;
};

VOLCANO_END

#endif // VOLCANO_OPENGLRESOURCE_HPP
