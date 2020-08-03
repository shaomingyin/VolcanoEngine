//
//
#include <functional>

using namespace std::placeholders;

#include <Volcano/OpenGLHeap.hpp>

#define VOLCANO_OPENGLHEAP_SIZE_ORDER 26
#define VOLCANO_OPENGLHEAP_SIZE (1 << VOLCANO_OPENGLHEAP_SIZE_ORDER)

VOLCANO_BEGIN

// OpenGLBuffer

class OpenGLBuffer: public QIODevice
{
    friend class OpenGLHeap;

public:
    typedef std::function<void (OpenGLBuffer *)> FreeFunction;

public:
    OpenGLBuffer(QOpenGLBuffer *heap, int offset, int size, FreeFunction freeFunction);
    ~OpenGLBuffer(void) override;

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

OpenGLBuffer::OpenGLBuffer(QOpenGLBuffer *heap, int offset, int size, FreeFunction freeFunction):
    m_heap(heap),
    m_offset(offset),
    m_size(size),
    m_pos(-1),
    m_freeFunction(freeFunction)
{
    Q_ASSERT(m_heap != nullptr);
    Q_ASSERT(0 <= offset && offset < VOLCANO_OPENGLHEAP_SIZE);
    Q_ASSERT(0 < size && size <= VOLCANO_OPENGLHEAP_SIZE);
}

OpenGLBuffer::~OpenGLBuffer(void)
{
    m_freeFunction(this);
}

bool OpenGLBuffer::atEnd(void) const
{
    return (m_pos == m_size);
}

void OpenGLBuffer::close(void)
{
    Q_ASSERT(m_pos >= 0);

    m_pos = -1;

    QIODevice::close();
}

bool OpenGLBuffer::isSequential(void) const
{
    return false;
}

bool OpenGLBuffer::open(OpenMode mode)
{
    Q_ASSERT(m_pos < 0);

    if (!QIODevice::open(mode))
        return false;

    m_pos = 0;

    return true;
}

qint64 OpenGLBuffer::pos(void) const
{
    return m_pos;
}

bool OpenGLBuffer::reset(void)
{
    m_pos = 0;
    return true;
}

bool OpenGLBuffer::seek(qint64 pos)
{
    if (pos < 0 || pos > m_size)
        return false;

    m_pos = pos;

    return true;
}

qint64 OpenGLBuffer::size(void) const
{
    return m_size;
}

qint64 OpenGLBuffer::readData(char *data, qint64 maxlen)
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

qint64 OpenGLBuffer::writeData(const char *data, qint64 maxSize)
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

// OpenGLHeap

OpenGLHeap::OpenGLHeap(QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage):
    m_heap(type)
{
    m_heap.setUsagePattern(usage);
}

OpenGLHeap::~OpenGLHeap(void)
{
}

bool OpenGLHeap::init(void)
{
    Q_ASSERT(!m_heap.isCreated());

    if (!m_heap.create())
        return false;

    m_heap.allocate(VOLCANO_OPENGLHEAP_SIZE);
    m_freeSize = VOLCANO_OPENGLHEAP_SIZE;

    return true;
}

QIODevice *OpenGLHeap::allocBuffer(int size)
{
    Q_ASSERT(size > 0);

    if (size > m_freeSize)
        return nullptr;

    int pos = 0;
    OpenGLBuffer *buf;
    OpenGLBuffer *used = nullptr;
    OpenGLBufferList::iterator it;

    for (it = m_bufferList.begin(); it != m_bufferList.end(); ++it)
    {
        used = *it;
        if ((used->m_offset - pos) >= size)
            break;
        pos = used->m_offset + used->m_size;
    }

    if (it == m_bufferList.end())
    {
        if (used != nullptr && size > (VOLCANO_OPENGLHEAP_SIZE - pos))
            return nullptr;
        buf = new OpenGLBuffer(&m_heap, pos, size, std::bind(&OpenGLHeap::freeBuffer, this, _1));
        m_bufferList.append(buf);
    }
    else
    {
        buf = new OpenGLBuffer(&m_heap, pos, size, std::bind(&OpenGLHeap::freeBuffer, this, _1));
        m_bufferList.insert(it, buf);
    }

    m_freeSize -= size;

    return buf;
}

void OpenGLHeap::freeBuffer(OpenGLBuffer *buf)
{
    Q_ASSERT(buf != NULL);
    Q_ASSERT(buf->m_heap == &m_heap);
    Q_ASSERT(buf->m_size > 0);
    Q_ASSERT(m_bufferList.contains(buf));

    m_bufferList.removeOne(buf);
    delete buf;
}

VOLCANO_END
