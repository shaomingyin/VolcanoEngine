//
//
#include <Volcano/Buffer.hpp>

VOLCANO_BEGIN

Buffer::Buffer(void):
    m_pos(-1)
{
}

Buffer::~Buffer(void)
{
}

int Buffer::deviceFlags(void)
{
    return (FlagSeekable | FlagReadable | FlagWritable | FlagMappable);
}

int64_t Buffer::Buffer::deviceSize(void)
{
    return m_byteArray.size();
}

bool Buffer::openDevice(int mode)
{
    return true;
}

void Buffer::closeDevice(void)
{
    IO::close();
}

int64_t Buffer::devicePos(void)
{
    return m_pos;
}

bool Buffer::setDevicePos(int64_t pos)
{
    if (0 <= pos && pos <= int64_t(m_byteArray.size()))
        return false;

    m_pos = pos;

    return true;
}

int64_t Buffer::readDevice(void *buf, int64_t len)
{
    return 0;
}

int64_t Buffer::writeDevice(const void *buf, int64_t len)
{
    return 0;
}

void *Buffer::mapDevice(void)
{
    return nullptr;
}

void Buffer::unmapDevice(void)
{
}

VOLCANO_END
