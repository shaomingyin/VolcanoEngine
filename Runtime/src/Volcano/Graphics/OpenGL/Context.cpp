//
//
#include <list>
#include <memory>
#include <functional>

#include <QFile>
#include <QScopeGuard>
#include <QThread>
#include <QMutexLocker>
#include <QPromise>
#include <QOpenGLVersionFunctionsFactory>
#include <QCoreApplication>

#include <Volcano/RunnableEvent.hpp>
#include <Volcano/Graphics/OpenGL/Context.hpp>

VOLCANO_GRAPHICS_OPENGL_BEGIN

Context::ContextHashTable Context::c_contextHashTable;
QMutex Context::c_contextHashTableMutex;

Context::Context(QOpenGLContext *glContext, QObject *parent):
    QObject(parent),
    m_context(glContext),
    m_surface(nullptr),
    m_debugLogger(nullptr),
    m_functions(nullptr),
    m_shaderProgramCache(64)
{
    Q_ASSERT(m_context != nullptr);
}

Context::~Context(void)
{
    // TODO release all heaps.

    if (m_debugLogger != nullptr)
        delete m_debugLogger;
}

Context *Context::current(void)
{
    auto glContext = QOpenGLContext::currentContext();
    if (Q_UNLIKELY(glContext == nullptr))
        return nullptr;

    QMutexLocker locker(&c_contextHashTableMutex);

    Context *context = c_contextHashTable.value(glContext, nullptr);
    if (Q_LIKELY(context != nullptr)) {
        Q_ASSERT(glContext == context->context());
        return context;
    }

    context = new Context(glContext);
    if (context == nullptr)
        return nullptr;

    QScopeGuard contextGuard([context] {
        delete context;
    });

    if (!context->init())
        return nullptr;

    QObject::connect(glContext, &QOpenGLContext::aboutToBeDestroyed, [context]() {
        QMutexLocker locker(&c_contextHashTableMutex);
        auto glContext = context->context();
        Q_ASSERT(glContext != nullptr);
        Q_ASSERT(c_contextHashTable.contains(glContext));
        Q_ASSERT(c_contextHashTable.value(glContext, nullptr) == context);
        c_contextHashTable.remove(glContext);
        delete context;
    });

    c_contextHashTable.insert(glContext, context);
    contextGuard.dismiss();

    return context;
}

QOpenGLContext *Context::context(void)
{
    return m_context;
}

QOpenGLDebugLogger *Context::debugLogger(void)
{
    return m_debugLogger;
}

Functions *Context::functions(void)
{
    return m_functions;
}

Buffer *Context::allocStaticVertexBuffer(GLsizeiptr count)
{
    return allocBuffer(m_staticVertexBufferHeapList, sizeof(Vertex) * count,
        QOpenGLBuffer::VertexBuffer, QOpenGLBuffer::StaticDraw);
}

Buffer *Context::allocStaticVertexIndexBuffer(GLsizeiptr count)
{
    return allocBuffer(m_staticVertexIndexBufferHeapList, sizeof(VertexIndex) * count,
        QOpenGLBuffer::IndexBuffer, QOpenGLBuffer::StaticDraw);
}

Buffer *Context::allocDynamicVertexBuffer(GLsizeiptr count)
{
    return allocBuffer(m_dynamicVertexBufferHeapList, sizeof(Vertex) * count,
        QOpenGLBuffer::VertexBuffer, QOpenGLBuffer::DynamicDraw);
}

Buffer *Context::allocDynamicVertexIndexBuffer(GLsizeiptr count)
{
    return allocBuffer(m_dynamicVertexIndexBufferHeapList, sizeof(VertexIndex) * count,
        QOpenGLBuffer::IndexBuffer, QOpenGLBuffer::DynamicDraw);
}

void Context::onDebugMessageLogged(const QOpenGLDebugMessage &msg)
{
    // TODO
    // qDebug() << msg.message();
}

bool Context::init(void)
{
    Q_ASSERT(m_context != nullptr);
    Q_ASSERT(m_context == QOpenGLContext::currentContext());
    Q_ASSERT(m_surface == nullptr);
    Q_ASSERT(m_debugLogger == nullptr);
    Q_ASSERT(m_functions == nullptr);

    m_surface = m_context->surface();
    Q_ASSERT(m_surface != nullptr);

    if (m_context->hasExtension(QByteArrayLiteral("GL_KHR_debug"))) {
        //m_glDebugLogger = new QOpenGLDebugLogger();
        //m_glDebugLogger->initialize();
        //m_glDebugLogger->startLogging();
        //connect(m_glDebugLogger, &QOpenGLDebugLogger::messageLogged, this, &Context::onDebugMessageLogged);
    }

    m_functions = QOpenGLVersionFunctionsFactory::get<Functions>();
    if (m_functions == nullptr)
        return false;

    return true;
}

Buffer *Context::allocBuffer(BufferHeapList &heapList, GLsizeiptr size, QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage)
{
    Q_ASSERT(size > 0);

    Buffer *buf;
    BufferHeapList::iterator it;

    for (it = heapList.begin(); it != heapList.end(); ++it) {
        buf = (*it)->allocBuffer(size);
        if (buf != nullptr)
            return buf;
    }

    auto heap = std::make_unique<BufferHeap>(type, usage);
    if (!heap || !heap->init())
        return nullptr;

    buf = heap->allocBuffer(size);
    heapList.prepend(heap.release());

    return buf;
}

QOpenGLShaderProgram *Context::shaderProgram(const QUrl &url)
{
    auto p = m_shaderProgramCache.object(url);
    if (p != nullptr)
        return p;

    auto shaderProgram = std::make_unique<QOpenGLShaderProgram>();
    if (!shaderProgram || !shaderProgram->create())
        return nullptr;

    // TODO load shaders from url

    if (!shaderProgram->link())
        return nullptr;

    p = shaderProgram.release();
    m_shaderProgramCache.insert(url, p);

    return p;
}

void Context::scheduleJob(Job job)
{
    QMutexLocker locker(&c_contextHashTableMutex);

    for (auto it = c_contextHashTable.begin(); it != c_contextHashTable.end(); ++it) {
        auto context = it.value();
        auto runnable = QRunnable::create([job, context] () {
            job(context);
        });
        runnable->setAutoDelete(true);
        QCoreApplication::postEvent(context, new RunnableEvent(runnable));
    }
}

void Context::customEvent(QEvent *event)
{
    if (event->type() == RunnableEvent::typeId()) {
        if (m_context != nullptr && m_surface != nullptr) {
            static_cast<RunnableEvent *>(event)->run();
        }
    }
}

VOLCANO_GRAPHICS_OPENGL_END
