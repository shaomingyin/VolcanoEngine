//
//
#include <algorithm>
#include <functional>

#include <QScopeGuard>

#include <Volcano/Graphics/Heap.hpp>

VOLCANO_GRAPHICS_BEGIN

// Buffer

class Buffer: public QIODevice {
    Q_OBJECT

public:
    std::function<void (QIODevice *)> freeBuffer;
    std::function<void (QIODevice *)> bindBuffer;
    std::function<void *(QIODevice *)> mapBuffer;
    std::function<void (QIODevice *)> unmapBuffer;

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
    m_map(nullptr),
    m_pos(-1)
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

// Heap

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

QIODevice *Heap::allocBuffer(GLsizeiptr size)
{
    Q_ASSERT(size > 0);
    Q_ASSERT(m_glBuffer.isCreated());

    if (size > m_freeSize)
        return nullptr;

    BufferList::iterator it;
    Buffer *used;
    qint64 offset = 0;

    for (it = m_bufferList.begin(); it != m_bufferList.end(); ++it) {
        used = static_cast<Buffer *>(*it);
        if ((used->offset() - offset) >= size)
            break;
        offset = used->offset() + used->size();
    }

    auto buffer = new Buffer(offset, size);
    if (buffer == nullptr)
        return nullptr;

    if (it == m_bufferList.end()) {
        if (used != nullptr && size > (m_glBuffer.size() - offset))
            return nullptr;
        m_bufferList.push_front(buffer);
    } else
        m_bufferList.insert(it, buffer);

    m_freeSize -= size;

    buffer->freeBuffer = [this](QIODevice *buf) { freeBuffer(buf); };
    buffer->bindBuffer = [this](QIODevice *buf) { bindBuffer(buf); };
    buffer->mapBuffer = [this](QIODevice *buf) { return mapBuffer(buf); };
    buffer->unmapBuffer = [this](QIODevice *buf) { unmapBuffer(buf); };

    return buffer;
}

void Heap::freeBuffer(QIODevice *buf)
{
    m_bufferList.remove(buf);
}

void Heap::bindBuffer(QIODevice *buf)
{
    Q_UNUSED(buf);

    m_glBuffer.bind();
}

void *Heap::mapBuffer(QIODevice *buf)
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

void Heap::unmapBuffer(QIODevice *buf)
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

#include <Heap.moc>
