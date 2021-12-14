//
//
#include <list>
#include <memory>
#include <functional>

#include <QFile>
#include <QScopeGuard>
#include <QMutexLocker>
#include <QOpenGLVersionFunctionsFactory>

#include <Volcano/Graphics/Context.hpp>

VOLCANO_GRAPHICS_BEGIN

Context::ContextHashTable Context::c_contextHashTable;
QMutex Context::c_contextHashTableMutex;

Context::Context(QOpenGLContext *glContext, QObject *parent):
    QObject(parent),
    m_glContext(glContext),
    m_glDebugLogger(nullptr),
    m_glFunctions(nullptr),
    m_shaderProgramCache(64)
{
    Q_ASSERT(m_glContext != nullptr);
}

Context::~Context(void)
{
    // TODO release all heaps.

    if (m_glDebugLogger != nullptr)
        delete m_glDebugLogger;
}

Context *Context::current(void)
{
    auto glContext = QOpenGLContext::currentContext();
    if (Q_UNLIKELY(glContext == nullptr))
        return nullptr;

    QMutexLocker locker(&c_contextHashTableMutex);

    Context *context = c_contextHashTable.value(glContext, nullptr);
    if (Q_LIKELY(context != nullptr)) {
        Q_ASSERT(glContext == context->glContext());
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
        auto glContext = context->glContext();
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

QOpenGLContext *Context::glContext(void)
{
    return m_glContext;
}

QOpenGLDebugLogger *Context::glDebugLogger(void)
{
    return m_glDebugLogger;
}

OpenGLFunctions *Context::glFunctions(void)
{
    return m_glFunctions;
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
    Q_ASSERT(m_glContext != nullptr);
    Q_ASSERT(m_glContext == QOpenGLContext::currentContext());
    Q_ASSERT(m_glDebugLogger == nullptr);
    Q_ASSERT(m_glFunctions == nullptr);

    if (m_glContext->hasExtension(QByteArrayLiteral("GL_KHR_debug"))) {
        //m_glDebugLogger = new QOpenGLDebugLogger();
        //m_glDebugLogger->initialize();
        //m_glDebugLogger->startLogging();
        //connect(m_glDebugLogger, &QOpenGLDebugLogger::messageLogged, this, &Context::onDebugMessageLogged);
    }

    m_glFunctions = QOpenGLVersionFunctionsFactory::get<OpenGLFunctions>();
    if (m_glFunctions == nullptr)
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

MeshPtr Context::mesh(const QUrl &url)
{
    if (!url.isValid())
        return nullptr;

    return nullptr;
}

void Context::gc(int flags)
{

}

VOLCANO_GRAPHICS_END
