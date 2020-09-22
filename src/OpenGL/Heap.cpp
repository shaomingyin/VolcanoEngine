//
//
#include <functional>

#include <Volcano/OpenGL/Heap.hpp>

VOLCANO_OPENGL_BEGIN

using namespace std::placeholders;

Heap::Heap(QOpenGLBuffer::Type type, QOpenGLBuffer::UsagePattern usage):
    m_heap(type),
    m_size(0),
    m_freeSize(0),
    m_map(nullptr),
    m_mapCount(0)
{
    m_heap.setUsagePattern(usage);
}

Heap::~Heap(void)
{
    Q_ASSERT(m_map == nullptr);
    Q_ASSERT(m_mapCount == 0);
}

bool Heap::init(int size)
{
    Q_ASSERT(size > 0);
    Q_ASSERT(!m_heap.isCreated());

    if (!m_heap.create())
        return false;

    m_heap.allocate(size);
    m_size = size;
    m_freeSize = size;

    return true;
}

Buffer *Heap::allocBuffer(int size)
{
    Q_ASSERT(size > 0);
    Q_ASSERT(m_heap.isCreated());

    Q_ASSERT(size > 0);
    Q_ASSERT(size <= m_size);

    if (size > m_freeSize)
        return nullptr;

    BufferList::iterator it;
    Buffer *used;
    int pos = 0;

    for (it = m_bufferList.begin(); it != m_bufferList.end(); ++it)
    {
        used = *it;
        if ((used->offset() - pos) >= size)
            break;
        pos = used->offset() + used->size();
    }

    Buffer *buf = new Buffer(pos, size);
    if (buf == nullptr)
        return nullptr;

    if (it == m_bufferList.end()) {
        if (used != nullptr && size > (m_size - pos))
            return nullptr;
        m_bufferList.prepend(buf);
    } else
        m_bufferList.insert(it, buf);

    m_freeSize -= size;

    buf->freeFunction = std::bind(&Heap::freeBuffer, this, buf);
    buf->mapFunction = std::bind(&Heap::mapBuffer, this, buf, _1);
    buf->unmapFunction = std::bind(&Heap::unmapBuffer, this, buf);

    return buf;
}

void Heap::freeBuffer(Buffer *buf)
{
    Q_ASSERT(m_bufferList.contains(buf));

    m_bufferList.removeOne(buf);
}

void *Heap::mapBuffer(Buffer *buf, QOpenGLBuffer::Access access)
{
    Q_ASSERT(m_bufferList.contains(buf));

    if (m_map == nullptr)
    {
        Q_ASSERT(m_mapCount == 0);
        m_heap.bind();
        m_map = m_heap.map(access);
        if (m_map == nullptr)
            return nullptr;
    }

    m_mapCount += 1;

    return m_map;
}

void Heap::unmapBuffer(Buffer *buf)
{
    Q_ASSERT(m_bufferList.contains(buf));
    Q_ASSERT(m_map != nullptr);
    Q_ASSERT(m_mapCount > 0);

    m_mapCount -= 1;
    if (m_mapCount > 0)
        return;

    m_heap.bind();
    m_heap.unmap();
    m_map = nullptr;
}

VOLCANO_OPENGL_END
