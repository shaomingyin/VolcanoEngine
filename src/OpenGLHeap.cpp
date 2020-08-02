//
//
#include <functional>

using namespace std::placeholders;

#include <Volcano/OpenGLHeap.hpp>

#define VOLCANO_OPENGLHEAP_SIZE_ORDER 26
#define VOLCANO_OPENGLHEAP_SIZE (1 << VOLCANO_OPENGLHEAP_SIZE_ORDER)

VOLCANO_BEGIN

// OpenGLHeapBuffer

class OpenGLHeapBuffer: public OpenGLBuffer
{
    friend class OpenGLHeap;

public:
    typedef std::function<void (OpenGLHeapBuffer *)> FreeFunction;

public:
    OpenGLHeapBuffer(QOpenGLBuffer *heap, int offset, int size, FreeFunction freeFunction);
    ~OpenGLHeapBuffer(void) override;

public:
    int size(void) override;

private:
    QOpenGLBuffer *m_heap;
    int m_offset;
    int m_size;
    FreeFunction m_freeFunction;
};

OpenGLHeapBuffer::OpenGLHeapBuffer(QOpenGLBuffer *heap, int offset, int size, FreeFunction freeFunction):
    m_heap(heap),
    m_offset(offset),
    m_size(size),
    m_freeFunction(freeFunction)
{
    Q_ASSERT(m_heap != nullptr);
    Q_ASSERT(0 <= offset && offset < VOLCANO_OPENGLHEAP_SIZE);
    Q_ASSERT(0 < size && size <= VOLCANO_OPENGLHEAP_SIZE);
}

OpenGLHeapBuffer::~OpenGLHeapBuffer(void)
{
    m_freeFunction(this);
}

int OpenGLHeapBuffer::size(void)
{
    return m_size;
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

OpenGLBuffer *OpenGLHeap::allocBuffer(int size)
{
    Q_ASSERT(size > 0);

    if (size > m_freeSize)
        return nullptr;

    int pos = 0;
    OpenGLHeapBuffer *buf;
    OpenGLHeapBuffer *used = nullptr;
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
        buf = new OpenGLHeapBuffer(&m_heap, pos, size, std::bind(&OpenGLHeap::freeBuffer, this, _1));
        m_bufferList.append(buf);
    }
    else
    {
        buf = new OpenGLHeapBuffer(&m_heap, pos, size, std::bind(&OpenGLHeap::freeBuffer, this, _1));
        m_bufferList.insert(it, buf);
    }

    m_freeSize -= size;

    return buf;
}

void OpenGLHeap::freeBuffer(OpenGLHeapBuffer *buf)
{
    Q_ASSERT(buf != NULL);
    Q_ASSERT(buf->m_heap == &m_heap);
    Q_ASSERT(buf->m_size > 0);
    Q_ASSERT(m_bufferList.contains(buf));

    m_bufferList.removeOne(buf);
    delete buf;
}

VOLCANO_END
