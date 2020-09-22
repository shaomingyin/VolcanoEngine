//
//
#include <Volcano/OpenGL/Buffer.hpp>

VOLCANO_OPENGL_BEGIN

Buffer::Buffer(int offset, int size, QObject *parent):
    QIODevice(parent),
    m_offset(offset),
    m_size(size),
    m_pos(0),
    m_map(nullptr)
{
}

Buffer::~Buffer(void)
{
    freeFunction();
}

bool Buffer::atEnd(void) const
{
    Q_ASSERT(m_map != nullptr);

    return (m_pos == m_size);
}

void Buffer::close(void)
{
    Q_ASSERT(m_map != nullptr);

    QIODevice::close();

    unmapFunction();
    m_map = nullptr;
}

bool Buffer::isSequential(void) const
{
    return false;
}

bool Buffer::open(QIODevice::OpenMode mode)
{
    Q_ASSERT(m_map == nullptr);

    if (mode & (Append | NewOnly | Truncate))
        return false;

    QOpenGLBuffer::Access acc;
    switch (mode)
    {
    case ReadOnly:
        acc = QOpenGLBuffer::ReadOnly;
        break;
    case WriteOnly:
        acc = QOpenGLBuffer::WriteOnly;
        break;
    case ReadWrite:
        acc = QOpenGLBuffer::ReadWrite;
        break;
    default:
        return false;
    }

    if (!QIODevice::open(mode))
        return false;

    m_map = mapFunction(acc);
    if (m_map == nullptr)
    {
        QIODevice::close();
        return false;
    }

    m_pos = 0;

    return true;
}

qint64 Buffer::pos(void) const
{
    Q_ASSERT(m_map != nullptr);

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

    if (pos < 0 || pos > m_size)
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

    qint64 len = qMin(maxSize, qint64(m_size - m_pos));
    if (len > 0)
        memcpy(data, ((quint8 *)m_map) + m_offset + m_pos, len);

    return len;
}

qint64 Buffer::writeData(const char *data, qint64 maxSize)
{
    Q_ASSERT(m_map != nullptr);

    qint64 len = qMin(maxSize, qint64(m_size - m_pos));
    if (len > 0)
        memcpy(((quint8 *)m_map) + m_offset + m_pos, data, len);

    return len;
}

VOLCANO_OPENGL_END
