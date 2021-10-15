//
//
#include <list>
#include <memory>
#include <functional>

#include <QOpenGLContext>
#include <QOpenGLVersionFunctionsFactory>

#include <QList>
#include <QHash>
#include <QMutex>
#include <QObject>
#include <QScopeGuard>
#include <QMutexLocker>
#include <QOpenGLContext>
#include <QOpenGLBuffer>

#include <Volcano/Graphics/Common.hpp>
#include <Volcano/Graphics/Buffer.hpp>

VOLCANO_GRAPHICS_BEGIN

class BufferImpl: public Buffer {
    Q_OBJECT

public:
    std::function<void (BufferImpl *)> freeBuffer;
    std::function<void (BufferImpl *)> bindBuffer;
    std::function<void *(BufferImpl *)> mapBuffer;
    std::function<void (BufferImpl *)> unmapBuffer;

public:
    BufferImpl(qint64 offset, qint64 size, QObject *parent = nullptr);
    ~BufferImpl(void) override;

public:
    bool atEnd(void) const override;
    bool canReadLine(void) const override;
    void close(void) override;
    bool isSequential() const override;
    bool open(OpenMode mode) override;
    qint64 pos(void) const override;
    bool reset(void) override;
    bool seek(qint64 pos) override;
    qint64 size(void) const override;
    void bind(void) override;
    qint64 offset(void) const;

protected:
    qint64 readData(char *data, qint64 maxSize) override;
    qint64 writeData(const char *data, qint64 maxSize) override;

private:
    qint64 m_offset;
    qint64 m_size;
    qint64 m_pos;
    void *m_map;
};

BufferImpl::BufferImpl(qint64 offset, qint64 size, QObject *parent):
    Buffer(parent),
    m_offset(offset),
    m_size(size),
    m_pos(-1),
    m_map(nullptr)
{
}

BufferImpl::~BufferImpl(void)
{
    freeBuffer(this);
}

bool BufferImpl::atEnd(void) const
{
    Q_ASSERT(m_map != nullptr);
    Q_ASSERT(0 <= m_pos && m_pos <= m_size);

    return (m_pos == m_size);
}

bool BufferImpl::canReadLine(void) const
{
    return false;
}

void BufferImpl::close(void)
{
    unmapBuffer(this);
}

bool BufferImpl::isSequential() const
{
    return false;
}

bool BufferImpl::open(OpenMode mode)
{
    Q_ASSERT(m_map == nullptr);

    if (mode & (Append | NewOnly | Truncate))
        return false;

    if (!QIODevice::open(mode))
        return false;

    m_map = mapBuffer(this);
    if (m_map == nullptr) {
        QIODevice::close();
        return false;
    }

    m_pos = 0;

    return true;
}

qint64 BufferImpl::pos(void) const
{
    Q_ASSERT(m_map != nullptr);
    Q_ASSERT(0 <= m_pos && m_pos <= m_size);

    return m_pos;
}

bool BufferImpl::reset(void)
{
    Q_ASSERT(m_map != nullptr);

    m_pos = 0;

    return true;
}

bool BufferImpl::seek(qint64 pos)
{
    Q_ASSERT(m_map != nullptr);

    if (pos < 0 || m_size < pos)
        return false;

    m_pos = pos;

    return true;
}

qint64 BufferImpl::size(void) const
{
    return m_size;
}

qint64 BufferImpl::readData(char *data, qint64 maxSize)
{
    Q_ASSERT(m_map != nullptr);
    Q_ASSERT(0 <= m_pos && m_pos <= m_size);

    auto readSize = qMin(m_size - m_pos, maxSize);
    if (readSize > 0) {
        memcpy(data, ((quint8 *)(m_map)) + m_pos, readSize);
        m_pos += readSize;
    }

    return readSize;
}

qint64 BufferImpl::writeData(const char *data, qint64 maxSize)
{
    Q_ASSERT(m_map != nullptr);
    Q_ASSERT(0 <= m_pos && m_pos <= m_size);

    auto writeSize = qMin(m_size - m_pos, maxSize);
    if (writeSize > 0) {
        memcpy(((quint8 *)(m_map)) + m_pos, data, writeSize);
        m_pos += writeSize;
    }

    return writeSize;
}

qint64 BufferImpl::offset(void) const
{
    return m_offset;
}

void BufferImpl::bind(void)
{
    bindBuffer(this);
}

///////////////////////////////////////////////////////////////////////////////

class BufferHeap {
public:
    BufferHeap(QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage);
    virtual ~BufferHeap(void);

public:
    bool init(int order = 26);
    BufferImpl *allocBuffer(GLsizeiptr size);

private:
    void freeBuffer(BufferImpl *buf);
    void bindBuffer(BufferImpl *buf);
    void *mapBuffer(BufferImpl *buf);
    void unmapBuffer(BufferImpl *buf);

private:
    using BufferList = std::list<BufferImpl *>;

private:
    QOpenGLBuffer m_glBuffer;
    QOpenGLBuffer::UsagePattern m_usage;
    BufferList m_bufferList;
    GLsizeiptr m_freeSize;
    void *m_map;
    int m_mapCount;
};

BufferHeap::BufferHeap(QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage):
    m_glBuffer(type),
    m_usage(usage),
    m_freeSize(0),
    m_map(nullptr),
    m_mapCount(0)
{
}

BufferHeap::~BufferHeap(void)
{
    Q_ASSERT(m_map == nullptr);
    Q_ASSERT(m_mapCount == 0);

    if (m_glBuffer.isCreated())
        m_glBuffer.destroy();
}

bool BufferHeap::init(int order)
{
    Q_ASSERT(order >= 0);
    Q_ASSERT(!m_glBuffer.isCreated());

    GLsizeiptr size = GLsizeiptr(1) << order;

    if (!m_glBuffer.create())
        return false;

    m_glBuffer.allocate(size);
    m_freeSize = size;

    return true;
}

BufferImpl *BufferHeap::allocBuffer(GLsizeiptr size)
{
    Q_ASSERT(size > 0);
    Q_ASSERT(m_glBuffer.isCreated());

    if (size > m_freeSize)
        return nullptr;

    BufferList::iterator it;
    BufferImpl *used = nullptr;
    qint64 offset = 0;

    for (it = m_bufferList.begin(); it != m_bufferList.end(); ++it) {
        used = *it;
        if ((used->offset() - offset) >= size)
            break;
        offset = used->offset() + used->size();
    }

    if (it == m_bufferList.end()) {
        if (used != nullptr && size > (m_glBuffer.size() - offset))
            return nullptr;
    }

    auto heapBuffer = new BufferImpl(offset, size);
    if (heapBuffer == nullptr)
        return nullptr;

    m_bufferList.insert(it, heapBuffer);
    m_freeSize -= size;

    heapBuffer->freeBuffer = [this](BufferImpl *buf) { freeBuffer(buf); };
    heapBuffer->bindBuffer = [this](BufferImpl *buf) { bindBuffer(buf); };
    heapBuffer->mapBuffer = [this](BufferImpl *buf) { return mapBuffer(buf); };
    heapBuffer->unmapBuffer = [this](BufferImpl *buf) { unmapBuffer(buf); };

    return heapBuffer;
}

void BufferHeap::freeBuffer(BufferImpl *buf)
{
    m_bufferList.remove(buf);
}

void BufferHeap::bindBuffer(BufferImpl *buf)
{
    Q_UNUSED(buf);

    m_glBuffer.bind();
}

void *BufferHeap::mapBuffer(BufferImpl *buf)
{
    Q_UNUSED(buf);

    if (m_map == nullptr) {
        Q_ASSERT(m_mapCount == 0);
        m_glBuffer.bind();
        m_map = m_glBuffer.map(QOpenGLBuffer::ReadWrite);
        if (m_map == nullptr)
            return nullptr;
    }

    m_mapCount += 1;

    return m_map;
}

void BufferHeap::unmapBuffer(BufferImpl *buf)
{
    Q_ASSERT(m_map != nullptr);
    Q_ASSERT(m_mapCount > 0);
    Q_UNUSED(buf);

    m_mapCount -= 1;
    if (m_mapCount > 0)
        return;

    m_glBuffer.bind();
    m_glBuffer.unmap();

    m_map = nullptr;
}

///////////////////////////////////////////////////////////////////////////////

using BufferHeapList = QList<BufferHeap *>;

class Context {
public:
    Context(QOpenGLContext *glContext);
    virtual ~Context(void);

public:
    QOpenGLContext *glContext(void);
    bool init(void);
    OpenGLFunctions *glFunctions(void);
    BufferImpl *allocStaticVertexBuffer(GLsizeiptr count);
    BufferImpl *allocStaticVertexIndexBuffer(GLsizeiptr count);
    BufferImpl *allocDynamicVertexBuffer(GLsizeiptr count);
    BufferImpl *allocDynamicVertexIndexBuffer(GLsizeiptr count);
    // OpenGLTexture *allocTexture(GLint width, GLint height, GLenum format);

private:
    BufferImpl *allocBuffer(BufferHeapList &heapList, GLsizeiptr size, QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage);

private:
    QOpenGLContext *m_glContext;
    OpenGLFunctions *m_glFunctions;
    BufferHeapList m_staticVertexBufferHeapList;
    BufferHeapList m_staticVertexIndexBufferHeapList;
    BufferHeapList m_dynamicVertexBufferHeapList;
    BufferHeapList m_dynamicVertexIndexBufferHeapList;
};

Context::Context(QOpenGLContext *glContext):
    m_glContext(glContext),
    m_glFunctions(nullptr)
{
    Q_ASSERT(m_glContext != nullptr);
}

Context::~Context(void)
{
    // TODO release all heaps.
}

QOpenGLContext *Context::glContext(void)
{
    return m_glContext;
}

bool Context::init(void)
{
    Q_ASSERT(m_glContext != nullptr);
    Q_ASSERT(m_glContext == QOpenGLContext::currentContext());
    Q_ASSERT(m_glFunctions == nullptr);

    m_glFunctions = QOpenGLVersionFunctionsFactory::get<OpenGLFunctions>();
    if (m_glFunctions == nullptr)
        return false;

    return true;
}

OpenGLFunctions *Context::glFunctions(void)
{
    return m_glFunctions;
}

BufferImpl *Context::allocStaticVertexBuffer(GLsizeiptr count)
{
    return allocBuffer(m_staticVertexBufferHeapList, sizeof(Vertex) * count,
        QOpenGLBuffer::VertexBuffer, QOpenGLBuffer::StaticDraw);
}

BufferImpl *Context::allocStaticVertexIndexBuffer(GLsizeiptr count)
{
    return allocBuffer(m_staticVertexIndexBufferHeapList, sizeof(VertexIndex) * count,
        QOpenGLBuffer::IndexBuffer, QOpenGLBuffer::StaticDraw);
}

BufferImpl *Context::allocDynamicVertexBuffer(GLsizeiptr count)
{
    return allocBuffer(m_dynamicVertexBufferHeapList, sizeof(Vertex) * count,
        QOpenGLBuffer::VertexBuffer, QOpenGLBuffer::DynamicDraw);
}

BufferImpl *Context::allocDynamicVertexIndexBuffer(GLsizeiptr count)
{
    return allocBuffer(m_dynamicVertexIndexBufferHeapList, sizeof(VertexIndex) * count,
        QOpenGLBuffer::IndexBuffer, QOpenGLBuffer::DynamicDraw);
}

BufferImpl *Context::allocBuffer(BufferHeapList &heapList, GLsizeiptr size, QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage)
{
    Q_ASSERT(size > 0);

    BufferImpl *buf;
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
    heapList.push_front(heap.release());

    return buf;
}

///////////////////////////////////////////////////////////////////////////////

using ContextHashTable = QHash<QOpenGLContext *, Context *>;

static ContextHashTable contextHashTable;
static QMutex contextHashTableMutex;

static Context *currentContext(void)
{
    auto glContext = QOpenGLContext::currentContext();
    if (glContext == nullptr)
        return nullptr;

    QMutexLocker locker(&contextHashTableMutex);

    Context *context = contextHashTable.value(glContext, nullptr);
    if (context != nullptr) {
        Q_ASSERT(glContext == context->glContext());
        return context;
    }

    context = new Context(glContext);
    if (context == nullptr)
        return nullptr;

    QScopeGuard contextGuard([context] { delete context; });

    if (!context->init())
        return nullptr;

    QObject::connect(glContext, &QOpenGLContext::aboutToBeDestroyed, [glContext, context]() {
        QMutexLocker locker(&contextHashTableMutex);
        Q_ASSERT(contextHashTable.contains(glContext));
        Q_ASSERT(contextHashTable.value(glContext, nullptr) == context);
        contextHashTable.remove(glContext);
        delete context;
    });

    contextHashTable.insert(glContext, context);
    contextGuard.dismiss();

    return context;
}

OpenGLFunctions *glFunctions(void)
{
    auto context = currentContext();
    if (context != nullptr)
        return context->glFunctions();
    return nullptr;
}

Buffer *allocStaticVertexBuffer(int count)
{
    auto context = currentContext();
    if (context != nullptr)
        return context->allocStaticVertexBuffer(count);
    return nullptr;
}

Buffer *allocStaticVertexIndexBuffer(int count)
{
    auto context = currentContext();
    if (context != nullptr)
        return context->allocStaticVertexIndexBuffer(count);
    return nullptr;
}

Buffer *allocDynamicVertexBuffer(int count)
{
    auto context = currentContext();
    if (context != nullptr)
        return context->allocDynamicVertexBuffer(count);
    return nullptr;
}

Buffer *allocDynamicVertexIndexBuffer(int count)
{
    auto context = currentContext();
    if (context != nullptr)
        return context->allocDynamicVertexIndexBuffer(count);
    return nullptr;
}

//Texture *glAllocTexture(int width, int height, GLenum format)
//{
//    return nullptr;
//}

// QOpenGLShaderProgram *shaderProgram(type ...)
// {
//    ...
// }

VOLCANO_GRAPHICS_END

#include "Common.moc"
