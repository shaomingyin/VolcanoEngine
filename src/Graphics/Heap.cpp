//
//
#include <functional>

#include <Volcano/Graphics/Heap.hpp>

#define VOLCANO_GRAPHICS_HEAP_SIZE_ORDER 26
#define VOLCANO_GRAPHICS_HEAP_SIZE (1 << VOLCANO_GRAPHICS_HEAP_SIZE_ORDER)

VOLCANO_GRAPHICS_BEGIN

using namespace std::placeholders;

// HeapBuffer

class HeapBuffer: public Buffer
{
    friend class Heap;

public:
    typedef std::function<void (HeapBuffer *)> FreeFunction;

public:
    HeapBuffer(QOpenGLBuffer *heap, int offset, int size, FreeFunction freeFunction);
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

protected:
    qint64 readData(char *data, qint64 maxlen) override;
    qint64 writeData(const char *data, qint64 maxSize) override;

private:
    QOpenGLBuffer *m_heap;
    int m_offset;
    int m_size;
    int m_pos;
    FreeFunction m_freeFunction;
};

HeapBuffer::HeapBuffer(QOpenGLBuffer *heap, int offset, int size, FreeFunction freeFunction):
    m_heap(heap),
    m_offset(offset),
    m_size(size),
    m_pos(-1),
    m_freeFunction(freeFunction)
{
    Q_ASSERT(m_heap != nullptr);
    Q_ASSERT(0 <= offset && offset < VOLCANO_GRAPHICS_HEAP_SIZE);
    Q_ASSERT(0 < size && size <= VOLCANO_GRAPHICS_HEAP_SIZE);
}

HeapBuffer::~HeapBuffer(void)
{
    m_freeFunction(this);
}

bool HeapBuffer::atEnd(void) const
{
    return (m_pos == m_size);
}

void HeapBuffer::close(void)
{
    Q_ASSERT(m_pos >= 0);

    m_pos = -1;

    Buffer::close();
}

bool HeapBuffer::isSequential(void) const
{
    return false;
}

bool HeapBuffer::open(OpenMode mode)
{
    Q_ASSERT(m_pos < 0);
    Q_ASSERT(m_heap != nullptr);

    if (!Buffer::open(mode))
        return false;

    //m_heap->

    m_pos = 0;

    return true;
}

qint64 HeapBuffer::pos(void) const
{
    return m_pos;
}

bool HeapBuffer::reset(void)
{
    m_pos = 0;
    return true;
}

bool HeapBuffer::seek(qint64 pos)
{
    if (pos < 0 || pos > m_size)
        return false;

    m_pos = pos;

    return true;
}

qint64 HeapBuffer::size(void) const
{
    return m_size;
}

qint64 HeapBuffer::readData(char *data, qint64 maxlen)
{
    Q_ASSERT(m_pos >= 0);

    qint64 len = m_size - m_pos;
    len = qMin(len, maxlen);
    if (len > 0) {
        m_heap->bind();
        m_heap->read(m_offset + m_pos, data, len);
    }

    return len;
}

qint64 HeapBuffer::writeData(const char *data, qint64 maxSize)
{
    Q_ASSERT(m_pos >= 0);

    qint64 len = m_size - m_pos;
    len = qMin(len, maxSize);
    if (len > 0) {
        m_heap->bind();
        m_heap->write(m_offset + m_pos, data, len);
    }

    return len;
}

// Heap

Heap::Heap(QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage):
    m_heap(type)
{
    m_heap.setUsagePattern(usage);
}

Heap::~Heap(void)
{
}

bool Heap::init(void)
{
    Q_ASSERT(!m_heap.isCreated());

    if (!m_heap.create())
        return false;

    m_heap.allocate(VOLCANO_GRAPHICS_HEAP_SIZE);
    m_freeSize = VOLCANO_GRAPHICS_HEAP_SIZE;

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
        if (used != nullptr && size > (VOLCANO_GRAPHICS_HEAP_SIZE - pos))
            return nullptr;
        buf = new HeapBuffer(&m_heap, pos, size, std::bind(&Heap::freeBuffer, this, _1));
        m_heapBufferList.append(buf);
    }
    else
    {
        buf = new HeapBuffer(&m_heap, pos, size, std::bind(&Heap::freeBuffer, this, _1));
        m_heapBufferList.insert(it, buf);
    }

    m_freeSize -= size;

    return buf;
}

void Heap::freeBuffer(HeapBuffer *buf)
{
    Q_ASSERT(buf != NULL);
    Q_ASSERT(buf->m_heap == &m_heap);
    Q_ASSERT(buf->m_size > 0);
    Q_ASSERT(m_heapBufferList.contains(buf));

    m_heapBufferList.removeOne(buf);
    delete buf;
}

VOLCANO_GRAPHICS_END
