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

#include <Volcano/OpenGLMemory.hpp>

VOLCANO_BEGIN

class Buffer: public QIODevice {
    Q_OBJECT

public:
    std::function<void (Buffer *)> freeBuffer;
    std::function<void (Buffer *)> bindBuffer;
    std::function<void *(Buffer *)> mapBuffer;
    std::function<void (Buffer *)> unmapBuffer;

public:
    Buffer(qint64 offset, qint64 size, QObject *parent = nullptr);
    ~Buffer(void) override;

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

Buffer::Buffer(qint64 offset, qint64 size, QObject *parent):
    QIODevice(parent),
    m_offset(offset),
    m_size(size),
    m_pos(-1),
    m_map(nullptr)
{
}

Buffer::~Buffer(void)
{
    freeBuffer(this);
}

bool Buffer::atEnd(void) const
{
    Q_ASSERT(m_map != nullptr);
    Q_ASSERT(0 <= m_pos && m_pos <= m_size);

    return (m_pos == m_size);
}

bool Buffer::canReadLine(void) const
{
    return false;
}

void Buffer::close(void)
{
    unmapBuffer(this);
}

bool Buffer::isSequential() const
{
    return false;
}

bool Buffer::open(OpenMode mode)
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

qint64 Buffer::pos(void) const
{
    Q_ASSERT(m_map != nullptr);
    Q_ASSERT(0 <= m_pos && m_pos <= m_size);

    return m_pos;
}

bool Buffer::reset(void)
{
    Q_ASSERT(m_map != nullptr);

    m_pos = 0;

    return true;
}

bool Buffer::seek(qint64 pos)
{
    Q_ASSERT(m_map != nullptr);

    if (pos < 0 || m_size < pos)
        return false;

    m_pos = pos;

    return true;
}

qint64 Buffer::size(void) const
{
    return m_size;
}

qint64 Buffer::readData(char *data, qint64 maxSize)
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

qint64 Buffer::writeData(const char *data, qint64 maxSize)
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

qint64 Buffer::offset(void) const
{
    return m_offset;
}

///////////////////////////////////////////////////////////////////////////////

class Heap {
public:
    Heap(QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage);
    virtual ~Heap(void);

public:
    bool init(int order = 26);
    Buffer *allocBuffer(GLsizeiptr size);

private:
    void freeBuffer(Buffer *buf);
    void bindBuffer(Buffer *buf);
    void *mapBuffer(Buffer *buf);
    void unmapBuffer(Buffer *buf);

private:
    using BufferList = std::list<Buffer *>;

private:
    QOpenGLBuffer m_glBuffer;
    QOpenGLBuffer::UsagePattern m_usage;
    BufferList m_bufferList;
    GLsizeiptr m_freeSize;
    void *m_map;
    int m_mapCount;
};

Heap::Heap(QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage):
    m_glBuffer(type),
    m_usage(usage),
    m_freeSize(0),
    m_map(nullptr),
    m_mapCount(0)
{
}

Heap::~Heap(void)
{
    Q_ASSERT(m_map == nullptr);
    Q_ASSERT(m_mapCount == 0);

    if (m_glBuffer.isCreated())
        m_glBuffer.destroy();
}

bool Heap::init(int order)
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

Buffer *Heap::allocBuffer(GLsizeiptr size)
{
    Q_ASSERT(size > 0);
    Q_ASSERT(m_glBuffer.isCreated());

    if (size > m_freeSize)
        return nullptr;

    BufferList::iterator it;
    Buffer *used = nullptr;
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

    auto buffer = new Buffer(offset, size);
    if (buffer == nullptr)
        return nullptr;

    m_bufferList.insert(it, buffer);
    m_freeSize -= size;

    buffer->freeBuffer = [this](Buffer *buf) { freeBuffer(buf); };
    buffer->bindBuffer = [this](Buffer *buf) { bindBuffer(buf); };
    buffer->mapBuffer = [this](Buffer *buf) { return mapBuffer(buf); };
    buffer->unmapBuffer = [this](Buffer *buf) { unmapBuffer(buf); };

    return buffer;
}

void Heap::freeBuffer(Buffer *buf)
{
    m_bufferList.remove(buf);
}

void Heap::bindBuffer(Buffer *buf)
{
    Q_UNUSED(buf);

    m_glBuffer.bind();
}

void *Heap::mapBuffer(Buffer *buf)
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

void Heap::unmapBuffer(Buffer *buf)
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

using HeapList = QList<Heap *>;

class MemoryContext {
public:
    MemoryContext(QOpenGLContext *glContext);
    virtual ~MemoryContext(void);

public:
    QOpenGLContext *glContext(void);
    Buffer *allocStaticVertices(GLsizeiptr count);
    Buffer *allocStaticVertexIndices(GLsizeiptr count);
    Buffer *allocDynamicVertices(GLsizeiptr count);
    Buffer *allocDynamicVertexIndices(GLsizeiptr count);

private:
    Buffer *allocBuffer(HeapList &heapList, GLsizeiptr size, QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage);

private:
    QOpenGLContext *m_glContext;
    HeapList m_staticVertexHeapList;
    HeapList m_staticVertexIndexHeapList;
    HeapList m_dynamicVertexHeapList;
    HeapList m_dynamicVertexIndexHeapList;
};

MemoryContext::MemoryContext(QOpenGLContext *glContext)
    : m_glContext(glContext)
{
}

MemoryContext::~MemoryContext(void)
{
    // TODO release all heaps.
}

QOpenGLContext *MemoryContext::glContext(void)
{
    return m_glContext;
}

Buffer *MemoryContext::allocStaticVertices(GLsizeiptr count)
{
    return allocBuffer(m_staticVertexHeapList, sizeof(Vertex) * count,
        QOpenGLBuffer::VertexBuffer, QOpenGLBuffer::StaticDraw);
}

Buffer *MemoryContext::allocStaticVertexIndices(GLsizeiptr count)
{
    return allocBuffer(m_staticVertexIndexHeapList, sizeof(VertexIndex) * count,
        QOpenGLBuffer::IndexBuffer, QOpenGLBuffer::StaticDraw);
}

Buffer *MemoryContext::allocDynamicVertices(GLsizeiptr count)
{
    return allocBuffer(m_dynamicVertexHeapList, sizeof(Vertex) * count,
        QOpenGLBuffer::VertexBuffer, QOpenGLBuffer::DynamicDraw);
}

Buffer *MemoryContext::allocDynamicVertexIndices(GLsizeiptr count)
{
    return allocBuffer(m_dynamicVertexIndexHeapList, sizeof(VertexIndex) * count,
        QOpenGLBuffer::IndexBuffer, QOpenGLBuffer::DynamicDraw);
}

Buffer *MemoryContext::allocBuffer(HeapList &heapList, GLsizeiptr size, QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage)
{
    Q_ASSERT(size > 0);

    Buffer *buf;
    HeapList::iterator it;

    for (it = heapList.begin(); it != heapList.end(); ++it) {
        buf = (*it)->allocBuffer(size);
        if (buf != nullptr)
            return buf;
    }

    auto heap = std::make_unique<Heap>(type, usage);
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

static MemoryContext *memoryContext(void)
{
    auto glContext = QOpenGLContext::currentContext();
    if (glContext == nullptr)
        return nullptr;

    QMutexLocker locker(&memoryContextHashTableMutex);

    MemoryContext *mc = memoryContextHashTable.value(glContext, nullptr);
    if (mc != nullptr)
        return mc;

    mc = new MemoryContext(glContext);
    if (mc == nullptr)
        return nullptr;

    QObject::connect(glContext, &QOpenGLContext::aboutToBeDestroyed, [glContext, mc]() {
        QMutexLocker locker(&memoryContextHashTableMutex);
        Q_ASSERT(memoryContextHashTable.contains(glContext));
        Q_ASSERT(memoryContextHashTable.value(glContext, nullptr) == mc);
        memoryContextHashTable.remove(glContext);
        delete mc;
    });

    memoryContextHashTable.insert(glContext, mc);

    return mc;
}

QIODevice *allocStaticVertices(int count)
{
    auto mc = memoryContext();
    if (mc != nullptr)
        return mc->allocStaticVertices(count);
    return nullptr;
}

QIODevice *allocStaticVertexIndices(int count)
{
    auto mc = memoryContext();
    if (mc != nullptr)
        return mc->allocStaticVertexIndices(count);
    return nullptr;
}

QIODevice *allocDynamicVertices(int count)
{
    auto mc = memoryContext();
    if (mc != nullptr)
        return mc->allocDynamicVertices(count);
    return nullptr;
}

QIODevice *allocDynamicVertexIndices(int count)
{
    auto mc = memoryContext();
    if (mc != nullptr)
        return mc->allocDynamicVertexIndices(count);
    return nullptr;
}

VOLCANO_END

#include "OpenGLMemory.moc"
