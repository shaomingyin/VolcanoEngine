//
//
#include <list>
#include <memory>
#include <functional>

#include <QList>
#include <QHash>
#include <QMutex>
#include <QObject>
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

class MemoryContext {
public:
    MemoryContext(void);
    virtual ~MemoryContext(void);

public:
    BufferImpl *allocStaticVertexBuffer(GLsizeiptr count);
    BufferImpl *allocStaticVertexIndexBuffer(GLsizeiptr count);
    BufferImpl *allocDynamicVertexBuffer(GLsizeiptr count);
    BufferImpl *allocDynamicVertexIndexBuffer(GLsizeiptr count);
    // OpenGLTexture *allocTexture(GLint width, GLint height, GLenum format);

private:
    BufferImpl *allocBuffer(BufferHeapList &heapList, GLsizeiptr size, QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage);

private:
    BufferHeapList m_staticVertexBufferHeapList;
    BufferHeapList m_staticVertexIndexBufferHeapList;
    BufferHeapList m_dynamicVertexBufferHeapList;
    BufferHeapList m_dynamicVertexIndexBufferHeapList;
};

MemoryContext::MemoryContext(void)
{
}

MemoryContext::~MemoryContext(void)
{
    // TODO release all heaps.
}

BufferImpl *MemoryContext::allocStaticVertexBuffer(GLsizeiptr count)
{
    return allocBuffer(m_staticVertexBufferHeapList, sizeof(Vertex) * count,
        QOpenGLBuffer::VertexBuffer, QOpenGLBuffer::StaticDraw);
}

BufferImpl *MemoryContext::allocStaticVertexIndexBuffer(GLsizeiptr count)
{
    return allocBuffer(m_staticVertexIndexBufferHeapList, sizeof(VertexIndex) * count,
        QOpenGLBuffer::IndexBuffer, QOpenGLBuffer::StaticDraw);
}

BufferImpl *MemoryContext::allocDynamicVertexBuffer(GLsizeiptr count)
{
    return allocBuffer(m_dynamicVertexBufferHeapList, sizeof(Vertex) * count,
        QOpenGLBuffer::VertexBuffer, QOpenGLBuffer::DynamicDraw);
}

BufferImpl *MemoryContext::allocDynamicVertexIndexBuffer(GLsizeiptr count)
{
    return allocBuffer(m_dynamicVertexIndexBufferHeapList, sizeof(VertexIndex) * count,
        QOpenGLBuffer::IndexBuffer, QOpenGLBuffer::DynamicDraw);
}

BufferImpl *MemoryContext::allocBuffer(BufferHeapList &heapList, GLsizeiptr size, QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage)
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

using MemoryContextHashTable = QHash<QOpenGLContext *, MemoryContext *>;

static MemoryContextHashTable memoryContextHashTable;
static QMutex memoryContextHashTableMutex;

static MemoryContext *currentMemoryContext(void)
{
    auto glContext = QOpenGLContext::currentContext();
    if (glContext == nullptr)
        return nullptr;

    QMutexLocker locker(&memoryContextHashTableMutex);

    MemoryContext *memoryContext = memoryContextHashTable.value(glContext, nullptr);
    if (memoryContext != nullptr)
        return memoryContext;

    memoryContext = new MemoryContext();
    if (memoryContext == nullptr)
        return nullptr;

    QObject::connect(glContext, &QOpenGLContext::aboutToBeDestroyed, [glContext, memoryContext]() {
        QMutexLocker locker(&memoryContextHashTableMutex);
        Q_ASSERT(memoryContextHashTable.contains(glContext));
        Q_ASSERT(memoryContextHashTable.value(glContext, nullptr) == memoryContext);
        memoryContextHashTable.remove(glContext);
        delete memoryContext;
    });

    memoryContextHashTable.insert(glContext, memoryContext);

    return memoryContext;
}

QIODevice *allocStaticVertexBuffer(int count)
{
    auto memoryContext = currentMemoryContext();
    if (memoryContext != nullptr)
        return memoryContext->allocStaticVertexBuffer(count);
    return nullptr;
}

QIODevice *allocStaticVertexIndexBuffer(int count)
{
    auto memoryContext = currentMemoryContext();
    if (memoryContext != nullptr)
        return memoryContext->allocStaticVertexIndexBuffer(count);
    return nullptr;
}

QIODevice *allocDynamicVertexBuffer(int count)
{
    auto memoryContext = currentMemoryContext();
    if (memoryContext != nullptr)
        return memoryContext->allocDynamicVertexBuffer(count);
    return nullptr;
}

QIODevice *allocDynamicVertexIndexBuffer(int count)
{
    auto memoryContext = currentMemoryContext();
    if (memoryContext != nullptr)
        return memoryContext->allocDynamicVertexIndexBuffer(count);
    return nullptr;
}

QIODevice *glAllocTexture(int width, int height, GLenum format)
{
    return nullptr;
}

VOLCANO_GRAPHICS_END

#include "Memory.moc"
