//
//
#include <QMap>
#include <QOpenGLContext>

#include <Volcano/OpenGL.hpp>

VOLCANO_OPENGL_BEGIN

typedef QMap<QOpenGLContext *, MemoryPtr> MemoryMap;
typedef QMap<QOpenGLContext *, RendererPtr> RendererMap;

static MemoryMap memoryMap;
static RendererMap rendererMap;

static void memoryRemove(QOpenGLContext *context)
{
    memoryMap.remove(context);
}

MemoryPtr defaultMemory(void)
{
    QOpenGLContext *context = QOpenGLContext::currentContext();
    if (context == nullptr)
        return MemoryPtr();

    if (memoryMap.contains(context))
        return memoryMap[context];

    MemoryPtr memory(new Memory());
    if (!memory || !memory->init())
        return MemoryPtr();

    memoryMap[context] = memory;

    QObject::connect(context, &QOpenGLContext::aboutToBeDestroyed,
        std::bind(&memoryRemove, context));

    return memory;
}

static void rendererRemove(QOpenGLContext *context)
{
    rendererMap.remove(context);
}

RendererPtr defaultRenderer(void)
{
    QOpenGLContext *context = QOpenGLContext::currentContext();
    if (context == nullptr)
        return RendererPtr();

    if (rendererMap.contains(context))
        return rendererMap[context];

    RendererPtr renderer(new Renderer());
    if (!renderer || !renderer->init())
        return RendererPtr();

    rendererMap[context] = renderer;

    QObject::connect(context, &QOpenGLContext::aboutToBeDestroyed,
        std::bind(&rendererRemove, context));

    return renderer;
}

VOLCANO_OPENGL_END
