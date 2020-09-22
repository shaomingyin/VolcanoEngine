//
//
#ifndef VOLCANO_OPENGL_RENDERER_HPP
#define VOLCANO_OPENGL_RENDERER_HPP

#include <Volcano/Snapshot.hpp>
#include <Volcano/OpenGL/Common.hpp>
#include <Volcano/OpenGL/Mesh.hpp>

VOLCANO_OPENGL_BEGIN

class VOLCANO_API Renderer
{
public:
    Renderer(void);
    virtual ~Renderer(void);

public:
    bool init(void);
    void render(const Snapshot &r);

private:
    Functions *m_gl;
};

VOLCANO_OPENGL_END

#endif // VOLCANO_OPENGL_RENDERER_HPP
