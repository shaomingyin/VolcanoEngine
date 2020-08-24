//
//
#include <functional>

#include <Volcano/OpenGL/Heap.hpp>

#define VOLCANO_OPENGL_HEAP_SIZE_ORDER 26
#define VOLCANO_OPENGL_HEAP_SIZE (1 << VOLCANO_OPENGL_HEAP_SIZE_ORDER)

VOLCANO_OPENGL_BEGIN

using namespace std::placeholders;

// HeapBuffer

class HeapBuffer: public Buffer
{
    friend class Heap;

public:
    typedef std::function<void (HeapBuffer *)> FreeFunction;
    typedef std::function<void *(HeapBuffer *)> MapFunction;
    typedef std::function<void (HeapBuffer *)> UnmapFunction;

    FreeFunction freeFunction;
    MapFunction mapFunction;
    UnmapFunction unmapFunction;

public:
    HeapBuffer(QOpenGLBuffer *heap, int offset, int size);
    ~HeapBuffer(void) override;

public:
    bool atEnd(void) const override;
    void close(void) override;
    bool isSequential(void) const override;
    bool open(OpenMode mode) override;
    qint64 pos(void) const override;
    bool reset(void) override;
    bool seek(qint64 pos) override;
    qint64 size(void) const override;
    void *map(void) override;
    void unmap(void) override;

protected:
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 maxSize) override;

private:
    QOpenGLBuffer *m_heap;
    int m_offset;
    int m_size;
    int m_pos;
    void *m_map;
    bool m_userMapping;
};

HeapBuffer::HeapBuffer(QOpenGLBuffer *heap, int offset, int size):
    m_heap(heap),
    m_offset(offset),
    m_size(size),
    m_pos(-1),
    m_map(nullptr),
    m_userMapping(false)
{
    Q_ASSERT(m_heap != nullptr);
    Q_ASSERT(0 <= offset && offset < VOLCANO_OPENGL_HEAP_SIZE);
    Q_ASSERT(0 < size && size <= VOLCANO_OPENGL_HEAP_SIZE);
}

HeapBuffer::~HeapBuffer(void)
{
    if (m_map != nullptr)
        close();

    freeFunction(this);
}

bool HeapBuffer::atEnd(void) const
{
    Q_ASSERT(m_map != nullptr);

    return (m_pos == m_size);
}

void HeapBuffer::close(void)
{
    Q_ASSERT(m_map != nullptr);
    Q_ASSERT(!m_userMapping);

    unmapFunction(this);
    m_map = nullptr;

    Buffer::close();
}

bool HeapBuffer::isSequential(void) const
{
    return false;
}

bool HeapBuffer::open(OpenMode mode)
{
    Q_ASSERT(m_map == nullptr);

    if (!Buffer::open(mode))
        return false;

    m_map = mapFunction(this);
    if (m_map == nullptr)
    {
        Buffer::close();
        return false;
    }

    m_pos = 0;

    return true;
}

qint64 HeapBuffer::pos(void) const
{
    Q_ASSERT(m_map != nullptr);

    return m_pos;
}

bool HeapBuffer::reset(void)
{
    Q_ASSERT(m_map != nullptr);

    m_pos = 0;

    return true;
}

bool HeapBuffer::seek(qint64 pos)
{
    Q_ASSERT(m_map != nullptr);

    if (pos < 0 || pos > m_size)
        return false;

    m_pos = pos;

    return true;
}

qint64 HeapBuffer::size(void) const
{
    return m_size;
}

void *HeapBuffer::map(void)
{
    Q_ASSERT(m_map != nullptr);
    Q_ASSERT(!m_userMapping);

    m_userMapping = true;

    return m_map;
}

void HeapBuffer::unmap(void)
{
    Q_ASSERT(m_map != nullptr);
    Q_ASSERT(m_userMapping);

    m_userMapping = false;
}

qint64 HeapBuffer::readData(char *data, qint64 maxlen)
{
    Q_ASSERT(m_map != nullptr);

    qint64 len = m_size - m_pos;
    len = qMin(len, maxlen);
    if (len > 0)
        memcpy(((quint8 *)m_map) + m_pos, data, len);

    return len;
}

qint64 HeapBuffer::writeData(const char *data, qint64 maxSize)
{
    Q_ASSERT(m_map != nullptr);

    qint64 len = m_size - m_pos;
    len = qMin(len, maxSize);
    if (len > 0)
        memcpy(((quint8 *)m_map) + m_pos, data, len);

    return len;
}

// Heap

Heap::Heap(QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage):
    m_heap(type),
    m_freeSize(0),
    m_map(nullptr),
    m_mapCount(0)
{
    m_heap.setUsagePattern(usage);
}

Heap::~Heap(void)
{
    Q_ASSERT(m_mapCount == 0);
    Q_ASSERT(m_map == nullptr);
}

bool Heap::init(void)
{
    Q_ASSERT(!m_heap.isCreated());

    if (!m_heap.create())
        return false;

    m_heap.allocate(VOLCANO_OPENGL_HEAP_SIZE);
    m_freeSize = VOLCANO_OPENGL_HEAP_SIZE;

    return true;
}

Buffer *Heap::allocBuffer(int size)
{
    Q_ASSERT(size > 0);

    if (size > m_freeSize)
        return nullptr;

    int pos = 0;
    HeapBuffer *buf;
    HeapBuffer *used = nullptr;
    HeapBufferList::iterator it;

    for (it = m_heapBufferList.begin(); it != m_heapBufferList.end(); ++it)
    {
        used = *it;
        if ((used->m_offset - pos) >= size)
            break;
        pos = used->m_offset + used->m_size;
    }

    if (it == m_heapBufferList.end())
    {
        if (used != nullptr && size > (VOLCANO_OPENGL_HEAP_SIZE - pos))
            return nullptr;
        buf = new HeapBuffer(&m_heap, pos, size);
        m_heapBufferList.append(buf);
    }
    else
    {
        buf = new HeapBuffer(&m_heap, pos, size);
        m_heapBufferList.insert(it, buf);
    }

    if (buf)
    {
        buf->freeFunction = std::bind(&Heap::freeBuffer, this, _1);
        buf->mapFunction = std::bind(&Heap::mapBuffer, this, _1);
        buf->unmapFunction = std::bind(&Heap::unmapBuffer, this, _1);
    }

    m_freeSize -= size;

    return buf;
}

void Heap::freeBuffer(HeapBuffer *buf)
{
    Q_ASSERT(buf != nullptr);
    Q_ASSERT(buf->m_heap == &m_heap);
    Q_ASSERT(buf->m_map == nullptr);
    Q_ASSERT(m_heapBufferList.contains(buf));

    m_heapBufferList.removeOne(buf);
    delete buf;
}

void *Heap::mapBuffer(HeapBuffer *buf)
{
    Q_ASSERT(buf != nullptr);
    Q_ASSERT(buf->m_heap == &m_heap);
    Q_ASSERT(buf->m_map == nullptr);
    Q_ASSERT(m_heapBufferList.contains(buf));

    if (m_mapCount < 1)
    {
        Q_ASSERT(m_mapCount == 0);
        Q_ASSERT(m_map == nullptr);

        m_heap.bind();
        m_map = m_heap.map(QOpenGLBuffer::ReadWrite);
        if (m_map == nullptr)
            return nullptr;
    }

    Q_ASSERT(m_mapCount >= 0);
    m_mapCount += 1;

    return (((quint8 *)m_map) + buf->m_offset);
}

void Heap::unmapBuffer(HeapBuffer *buf)
{
    Q_ASSERT(buf != nullptr);
    Q_ASSERT(buf->m_heap == &m_heap);
    Q_ASSERT(buf->m_map != nullptr);
    Q_ASSERT(m_heapBufferList.contains(buf));
    Q_ASSERT(m_mapCount > 0);
    Q_ASSERT(m_map != nullptr);

    m_mapCount -= 1;
    if (m_mapCount > 0)
        return;

    m_heap.bind();
    m_heap.unmap();
    m_map = nullptr;
}

VOLCANO_OPENGL_END
