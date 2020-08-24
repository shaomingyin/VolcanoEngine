//
//
#include <functional>

#include <QMap>
#include <QOpenGLContext>
#include <QThread>
#include <QMutex>
#include <QMutexLocker>
#include <QObject>

#include <Volcano/OpenGL.hpp>

VOLCANO_OPENGL_BEGIN

typedef QMap<QOpenGLContext *, MemoryPtr> MemoryMap;
typedef QMap<QOpenGLContext *, RendererPtr> RendererMap;

static MemoryMap memoryMap;
static RendererMap rendererMap;
static QMutex m_mutex;

static void removeMemory(QOpenGLContext *context)
{
    QMutexLocker locker(&m_mutex);

    memoryMap.remove(context);
}

VOLCANO_API MemoryPtr defaultMemory(void)
{
    QOpenGLContext *context = QOpenGLContext::currentContext();
    Q_ASSERT(context != nullptr);
    Q_ASSERT(context->thread() == QThread::currentThread());

    QMutexLocker locker(&m_mutex);

    if (memoryMap.contains(context))
        return memoryMap.value(context);

    MemoryPtr memory(new Memory());
    if (!memory || !memory->init())
        return MemoryPtr();

    memoryMap[context] = memory;

    QObject::connect(context, &QOpenGLContext::aboutToBeDestroyed,
        std::bind(&removeMemory, context));

    return memory;
}

static void removeRenderer(QOpenGLContext *context)
{
    QMutexLocker locker(&m_mutex);

    rendererMap.remove(context);
}

VOLCANO_API RendererPtr defaultRenderer(void)
{
    QOpenGLContext *context = QOpenGLContext::currentContext();
    Q_ASSERT(context != nullptr);
    Q_ASSERT(context->thread() == QThread::currentThread());

    QMutexLocker locker(&m_mutex);

    if (rendererMap.contains(context))
        return rendererMap.value(context);

    RendererPtr renderer(new Renderer());
    if (!renderer || !renderer->init())
        return RendererPtr();

    rendererMap[context] = renderer;

    QObject::connect(context, &QOpenGLContext::aboutToBeDestroyed,
        std::bind(&removeRenderer, context));

    return renderer;
}

VOLCANO_OPENGL_END
