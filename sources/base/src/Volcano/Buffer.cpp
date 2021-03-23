//
//
#include <algorithm>

#include <Volcano/Buffer.hpp>

VOLCANO_BEGIN

Buffer::Buffer(void):
    m_pos(-1)
{
}

Buffer::Buffer(size_t size):
    m_byteArray(size),
    m_pos(-1)
{
}

Buffer::Buffer(const ByteArray &byteArray):
    m_byteArray(byteArray),
    m_pos(-1)
{
}

Buffer::Buffer(const Buffer &that):
    m_byteArray(that.m_byteArray),
    m_pos(-1)
{
}

Buffer::~Buffer(void)
{
}

int Buffer::flags(void)
{
    return (FlagSeekable | FlagReadable | FlagWritable);
}

int64_t Buffer::Buffer::size(void)
{
    return m_byteArray.size();
}

bool Buffer::open(int mode)
{
    if (!IO::open(mode))
        return false;

    m_pos = 0;

    return true;
}

void Buffer::close(void)
{
    IO::close();
    m_pos = -1;
}

bool Buffer::isEof(void)
{
    VOLCANO_ASSERT(isOpen());

    return (m_pos == m_byteArray.size());
}

const ByteArray &Buffer::toByteArray(void) const
{
    return m_byteArray;
}

Buffer &Buffer::operator=(const Buffer &that)
{
    if (this != &that) {
        m_byteArray = that.m_byteArray;
        m_pos = that.m_pos;
    }

    return (*this);
}

int64_t Buffer::pos(void)
{
    return m_pos;
}

bool Buffer::setPos(int64_t pos)
{
    if (0 <= pos && pos <= int64_t(m_byteArray.size()))
        return false;

    m_pos = pos;

    return true;
}

int64_t Buffer::readData(void *buf, int64_t len)
{
    VOLCANO_ASSERT(len > 0);

    int64_t readLen = m_byteArray.size() - m_pos;
    readLen = std::min(readLen, len);
    if (readLen > 0) {
        if (buf != nullptr)
            memcpy(buf, m_byteArray.data() + m_pos, readLen);
        m_pos += readLen;
    }

    return readLen;
}

int64_t Buffer::writeData(const void *buf, int64_t len)
{
    VOLCANO_ASSERT(buf != nullptr);
    VOLCANO_ASSERT(len > 0);

    int64_t writeLen = m_byteArray.size() - m_pos;
    writeLen = std::min(writeLen, len);
    if (writeLen > 0) {
        memcpy(m_byteArray.data() + m_pos, buf, writeLen);
        m_pos += writeLen;
    }

    return writeLen;
}

VOLCANO_END
