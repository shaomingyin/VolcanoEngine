//
//
#include <algorithm>
#include <functional>

#include <Volcano/ScopeGuard.hpp>
#include <Volcano/UI/Graphics/Heap.hpp>

VOLCANO_UI_GRAPHICS_BEGIN

// heap_buffer

class HeapBuffer: public Buffer {
public:
    std::function<void (void)> freeFunction;
    std::function<void (void)> bindFunction;
    std::function<void *(void)> mapFunction;
    std::function<void (void)> unmapFunction;

public:
    HeapBuffer(int64_t offset, int64_t size);
    ~HeapBuffer(void) override;

public:
    int64_t offset(void) const;

protected:
    int deviceFlags(void) override;
    int64_t deviceSize(void) override;
    bool openDevice(int mode) override;
    void closeDevice(void) override;
    int64_t devicePos(void) override;
    bool setDevicePos(int64_t pos) override;
    int64_t readDevice(void *buf, int64_t len) override;
    int64_t writeDevice(const void *buf, int64_t len) override;
    void *mapDevice(void) override;
    void unmapDevice(void) override;
    void bind(void) override;

private:
    int64_t m_offset;
    int64_t m_size;
    int64_t m_pos;
    void *m_map;
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

int HeapBuffer::deviceFlags(void)
{
    return (FlagReadable | FlagWritable | FlagSeekable | FlagMappable);
}

int64_t HeapBuffer::deviceSize(void)
{
    return m_size;
}

bool HeapBuffer::openDevice(int mode)
{
    VOLCANO_ASSERT(!isOpen());

    ScopeGuard closeGuard([=] { closeDevice(); });

    m_map = mapFunction();
    if (m_map == nullptr)
        return false;

    m_pos = 0;

    closeGuard.dismiss();

    return true;
}

void HeapBuffer::closeDevice(void)
{
    VOLCANO_ASSERT(m_map != nullptr);

    unmapFunction();
    m_map = nullptr;
}

int64_t HeapBuffer::devicePos(void)
{
    VOLCANO_ASSERT(isOpen());

    return m_pos;
}

bool HeapBuffer::setDevicePos(int64_t pos)
{
    VOLCANO_ASSERT(isOpen());

    m_pos = pos;

    return true;
}

void HeapBuffer::bind(void)
{
    bindFunction();
}

int64_t HeapBuffer::offset(void) const
{
    return m_offset;
}

int64_t HeapBuffer::readDevice(void *data, int64_t maxSize)
{
    VOLCANO_ASSERT(isOpen());

    int64_t len = std::min(m_size - m_offset - m_pos, maxSize);
    if (len > 0) {
        memcpy(data, ((uint8_t *)m_map) + m_offset + m_pos, len);
        m_pos += len;
    }

    return len;
}

int64_t HeapBuffer::writeDevice(const void *data, int64_t maxSize)
{
    VOLCANO_ASSERT(isOpen());

    int64_t len = std::min(m_size - -m_offset - m_pos, maxSize);
    if (len > 0) {
        memcpy(((uint8_t *)m_map) + m_offset + m_pos, data, len);
        m_pos += len;
    }

    return len;
}

void *HeapBuffer::mapDevice(void)
{
    return nullptr;
}

void HeapBuffer::unmapDevice(void)
{
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

    GLsizeiptr size = GLsizeiptr(1) << order;

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
    int64_t offset = 0;

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
#if 0
        VOLCANO_ASSERT(m_mapCount == 0);
        m_glBuffer.bind();
        m_map = m_glBuffer.map(QOpenGLBuffer::ReadWrite);
        if (m_map == nullptr)
            return nullptr;
#endif
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

VOLCANO_UI_GRAPHICS_END
