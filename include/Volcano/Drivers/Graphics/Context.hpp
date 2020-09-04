//
//
#ifndef VOLCANO_DRIVERS_GRAPHICS_CONTEXT_HPP
#define VOLCANO_DRIVERS_GRAPHICS_CONTEXT_HPP

#include <QObject>
#include <QOffscreenSurface>

#include <Volcano/Drivers/Graphics/Common.hpp>
#include <Volcano/Drivers/Graphics/Memory.hpp>
#include <Volcano/Drivers/Graphics/Renderer.hpp>

VOLCANO_DRIVERS_GRAPHICS_BEGIN

class VOLCANO_API Context: public QObject
{
public:
    Context(QObject *parent = nullptr);
    virtual ~Context(void);

public:
    bool init(void);
    Memory &memory(void);
    Renderer &renderer(void);

private:
    QOpenGLContext *m_glContext;
    Memory m_memory;
    Renderer m_renderer;
    QOffscreenSurface m_releaseSurface;
};

VOLCANO_INLINE Memory &Context::memory(void)
{
    return m_memory;
}

VOLCANO_INLINE Renderer &Context::renderer(void)
{
    return m_renderer;
}

VOLCANO_DRIVERS_GRAPHICS_END

#endif // VOLCANO_DRIVERS_GRAPHICS_CONTEXT_HPP
