//
//
#include <functional>

#include <QScopeGuard>

#include <Volcano/Graphics/Heap.hpp>

VOLCANO_GRAPHICS_BEGIN

// heap_buffer

class HeapBuffer: public Buffer {
public:
    std::function<void (void)> freeFunction;
    std::function<void (void)> bindFunction;
    std::function<void *(void)> mapFunction;
    std::function<void (void)> unmapFunction;

public:
    HeapBuffer(GLintptr offset, GLsizeiptr size);
    ~HeapBuffer(void) override;

public:
    bool atEnd(void) const override;
    void close(void) override;
    bool isSequential(void) const override;
    bool open(OpenMode mode) override;
    qint64 pos(void) const override;
    bool reset(void) override;
    bool seek(qint64 pos) override;
    void bind(void) override;
    GLintptr offset(void) const;

protected:
    qint64 readData(char *data, qint64 maxSize) override;
    qint64 writeData(const char *data, qint64 maxSize) override;

private:
    GLintptr m_offset;
    GLsizeiptr m_size;
    void *m_map;
    int m_pos;
};

HeapBuffer::HeapBuffer(GLintptr offset, GLsizeiptr size):
    m_offset(offset),
    m_size(size),
    m_map(nullptr),
    m_pos(-1)
{
}

HeapBuffer::~HeapBuffer(void)
{
    if (isOpen())
        close();
}

bool HeapBuffer::atEnd(void) const
{
    return true;
}

void HeapBuffer::close(void)
{
    Q_ASSERT(m_map != nullptr);

    unmapFunction();
    m_map = nullptr;
}

bool HeapBuffer::isSequential(void) const
{
    return false;
}

bool HeapBuffer::open(OpenMode mode)
{
    Q_ASSERT(!isOpen());

    if (!QIODevice::open(mode))
        return false;

    auto openGuard = qScopeGuard([=](){ close(); });

    m_map = mapFunction();
    if (m_map == nullptr)
        return false;

    m_pos = 0;

    openGuard.dismiss();

    return true;
}

qint64 HeapBuffer::pos(void) const
{
    Q_ASSERT(isOpen());

    return m_pos;
}

bool HeapBuffer::reset(void)
{
    Q_ASSERT(isOpen());

    m_pos = 0;


    return true;
}

bool HeapBuffer::seek(qint64 pos)
{
    Q_ASSERT(isOpen());

    if (0 <= pos && pos < m_size) {
        m_pos = pos;
        return true;
    }

    return false;
}

void HeapBuffer::bind(void)
{
    bindFunction();
}

GLintptr HeapBuffer::offset(void) const
{
    return m_offset;
}

qint64 HeapBuffer::readData(char *data, qint64 maxSize)
{
    Q_ASSERT(isOpen());

    qint64 len = qMin(m_size - m_offset - m_pos, maxSize);
    if (len > 0) {
        memcpy(data, ((quint8 *)m_map) + m_offset + m_pos, len);
        m_pos += len;
    }

    return len;
}

qint64 HeapBuffer::writeData(const char *data, qint64 maxSize)
{
    Q_ASSERT(isOpen());

    qint64 len = qMin(m_size - -m_offset - m_pos, maxSize);
    if (len > 0) {
        memcpy(((quint8 *)m_map) + m_offset + m_pos, data, len);
        m_pos += len;
    }

    return len;
}

// heap

Heap::Heap(QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage, QObject *parent):
    QObject(parent),
    m_glBuffer(type),
    m_freeSize(0),
    m_map(nullptr),
    m_mapCount(0)
{
    m_glBuffer.setUsagePattern(usage);
}

Heap::~Heap(void)
{
    Q_ASSERT(m_map == nullptr);
    Q_ASSERT(m_mapCount == 0);
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

void Heap::release(void)
{
    Q_ASSERT(m_bufferList.empty());

    m_glBuffer.destroy();
}

Buffer *Heap::allocBuffer(GLsizeiptr size)
{
    Q_ASSERT(size > 0);
    Q_ASSERT(m_glBuffer.isCreated());

    if (size > m_freeSize)
        return nullptr;

    BufferList::iterator it;
    HeapBuffer *used;
    int offset = 0;

    for (it = m_bufferList.begin(); it != m_bufferList.end(); ++it) {
        used = static_cast<HeapBuffer *>(*it);
        if ((used->offset() - offset) >= size)
            break;
        offset = used->offset() + used->size();
    }

    auto buf = new HeapBuffer(offset, size);
    if (buf == nullptr)
        return nullptr;

    if (it == m_bufferList.end()) {
        if (used != nullptr && size > (m_glBuffer.size() - offset))
            return nullptr;
        m_bufferList.push_front(buf);
    } else
        m_bufferList.insert(it, buf);

    m_freeSize -= size;

    buf->freeFunction = std::bind(&Heap::freeBuffer, this, buf);
    buf->bindFunction = std::bind(&Heap::bindBuffer, this, buf);
    buf->mapFunction = std::bind(&Heap::mapBuffer, this, buf);
    buf->unmapFunction = std::bind(&Heap::unmapBuffer, this, buf);

    return buf;
}

void Heap::freeBuffer(Buffer *buf)
{
    m_bufferList.removeOne(buf);
}

void Heap::bindBuffer(Buffer *buf)
{
    Q_UNUSED(buf);

    m_glBuffer.bind();
}

void *Heap::mapBuffer(Buffer *buf)
{
    Q_UNUSED(buf);

    if (m_map == nullptr)
    {
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

VOLCANO_GRAPHICS_END
