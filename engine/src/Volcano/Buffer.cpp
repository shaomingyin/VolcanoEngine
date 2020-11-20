//
//
#include <algorithm>

#include <Volcano/Buffer.hpp>

VOLCANO_BEGIN

Buffer::Buffer(void):
    m_pos(0)
{
}

Buffer::Buffer(const Buffer &that):
    data(that.data),
    m_pos(0)
{
}

Buffer::~Buffer(void)
{
}

bool Buffer::open(int flags)
{
    if (!IO::open(flags))
        return false;

    setSeekable(true);

    return true;
}

int64_t Buffer::size(void)
{
    return data.size();
}

int64_t Buffer::read(void *p, int64_t size)
{
    int64_t len = std::min(int64_t(data.size() - m_pos), size);
    if (len > 0) {
        memcpy(p, data.data() + m_pos, len);
        m_pos += len;
    }

    return len;
}

int64_t Buffer::write(const void *p, int64_t size)
{
    int64_t len = std::min(int64_t(data.size() - m_pos), size);
    if (len > 0) {
        memcpy(data.data() + m_pos, p, len);
        m_pos += len;
    }

    return len;
}

int64_t Buffer::pos(void)
{
    return m_pos;
}

int64_t Buffer::setPos(int64_t pos)
{
    m_pos = std::clamp(pos, int64_t(0), int64_t(data.size()));
    return m_pos;
}

VOLCANO_END
