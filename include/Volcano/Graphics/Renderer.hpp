//
//
#ifndef VOLCANO_GRAPHICS_RENDERER_HPP
#define VOLCANO_GRAPHICS_RENDERER_HPP

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Target.hpp>
#include <Volcano/Graphics/Mesh.hpp>

VOLCANO_GRAPHICS_BEGIN

class VOLCANO_API Renderer
{
public:
    Renderer(void);
    virtual ~Renderer(void);

public:
    virtual bool init(void) = 0;
    virtual Mesh *createMesh(int vertexCount, int vertexIndexCount, Mesh::Type type) = 0;
    virtual Target *createTarget(int width, int height) = 0;
    Target *target(void);
    const Target *target(void) const;
    void setTarget(Target *p);
    virtual void beginFrame(void) = 0;
    virtual void endFrame(void) = 0;

private:
    Target *m_target;
};

VOLCANO_INLINE Target *Renderer::target(void)
{
    return m_target;
}

VOLCANO_INLINE const Target *Renderer::target(void) const
{
    return m_target;
}

VOLCANO_INLINE void Renderer::setTarget(Target *p)
{
    m_target = p;
}

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_RENDERER_HPP
