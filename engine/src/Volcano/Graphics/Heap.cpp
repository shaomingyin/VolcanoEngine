//
//
#include <algorithm>
#include <functional>

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
    bool open(int flags) override;
    void close(void) override;
    int64_t size(void) override;
    int64_t read(void *p, int64_t size) override;
    int64_t write(const void *p, int64_t size) override;
    void bind(void) override;
    GLintptr offset(void) const;

protected:
    int64_t pos(void) override;
    int64_t setPos(int64_t pos) override;

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

bool HeapBuffer::open(int flags)
{
    VOLCANO_ASSERT(m_map == nullptr);

    if (!Buffer::open(flags))
        return false;

    m_map = mapFunction();
    if (m_map == nullptr)
        return false;

    setSeekable(true);
    m_pos = 0;

    return true;
}

void HeapBuffer::close(void)
{
    VOLCANO_ASSERT(m_map != nullptr);

    unmapFunction();
    m_map = nullptr;
}

int64_t HeapBuffer::size(void)
{
    return m_size;
}

int64_t HeapBuffer::read(void *p, int64_t size)
{
    VOLCANO_ASSERT(m_map != nullptr);
    VOLCANO_ASSERT(isReadable());

    int64_t len = std::min(m_size - m_offset - m_pos, size);
    if (len > 0) {
        memcpy(p, ((uint8_t *)m_map) + m_offset + m_pos, len);
        m_pos += len;
    }

    return len;
}

int64_t HeapBuffer::write(const void *p, int64_t size)
{
    VOLCANO_ASSERT(m_map != nullptr);
    VOLCANO_ASSERT(isWritable());

    int64_t len = std::min(m_size - -m_offset - m_pos, size);
    if (len > 0) {
        memcpy(((uint8_t *)m_map) + m_offset + m_pos, p, len);
        m_pos += len;
    }

    return len;
}

void HeapBuffer::bind(void)
{
    bindFunction();
}

int64_t HeapBuffer::pos(void)
{
    VOLCANO_ASSERT(m_map != nullptr);
    VOLCANO_ASSERT(isSeekable());

    return m_pos;
}

int64_t HeapBuffer::setPos(int64_t pos)
{
    VOLCANO_ASSERT(m_map != nullptr);
    VOLCANO_ASSERT(isSeekable());

    m_pos = std::clamp(pos, int64_t(0), m_size);

    return m_pos;
}

VOLCANO_INLINE GLintptr HeapBuffer::offset(void) const
{
    return m_offset;
}

// heap

Heap::Heap(GLenum target, GLenum usage):
    m_glBuffer(target, usage),
    m_freeSize(0),
    m_map(nullptr),
    m_mapCount(0)
{
}

Heap::~Heap(void)
{
    VOLCANO_ASSERT(m_map == nullptr);
    VOLCANO_ASSERT(m_mapCount == 0);
}

bool Heap::init(int order)
{
    VOLCANO_ASSERT(order >= 0);
    VOLCANO_ASSERT(!m_glBuffer.isValid());

    GLsizeiptr size = 1 << order;

    if (!m_glBuffer.create(size))
        return false;

    m_freeSize = size;

    return true;
}

void Heap::release(void)
{
    VOLCANO_ASSERT(m_bufferList.empty());
    m_glBuffer.destroy();
}

Buffer *Heap::allocBuffer(GLsizeiptr size)
{
    VOLCANO_ASSERT(size > 0);
    VOLCANO_ASSERT(m_glBuffer.isValid());

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
    m_bufferList.remove(buf);
}

void Heap::bindBuffer(Buffer *buf)
{
    VOLCANO_UNUSED(buf);

    m_glBuffer.bind();
}

void *Heap::mapBuffer(Buffer *buf)
{
    VOLCANO_UNUSED(buf);

    if (m_map == nullptr)
    {
        VOLCANO_ASSERT(m_mapCount == 0);
        m_glBuffer.bind();
        m_map = m_glBuffer.map();
        if (m_map == nullptr)
            return nullptr;
    }

    m_mapCount += 1;

    return m_map;
}

void Heap::unmapBuffer(Buffer *buf)
{
    VOLCANO_ASSERT(m_map != nullptr);
    VOLCANO_ASSERT(m_mapCount > 0);
    VOLCANO_UNUSED(buf);

    m_mapCount -= 1;
    if (m_mapCount > 0)
        return;

    m_glBuffer.bind();
    m_glBuffer.unmap();

    m_map = nullptr;
}

VOLCANO_GRAPHICS_END
