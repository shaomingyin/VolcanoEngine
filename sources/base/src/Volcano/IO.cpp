//
//
#include <algorithm>

#include <Volcano/IO.hpp>

VOLCANO_BEGIN

IO::IO(void):
    m_mode(0),
    m_map(nullptr)
{
}

IO::~IO(void)
{
}

int IO::flags(void)
{
    return deviceFlags();
}

int64_t IO::size(void)
{
    return deviceSize();
}

bool IO::open(int mode)
{
    VOLCANO_ASSERT(!isOpen());

    VOLCANO_ASSERT(mode != 0);

    if (mode & (ModeWrite | ModeCreate | ModeAppend | ModeTruncate)) {
        if (!(flags() & FlagWritable))
            return false;
    }

    if (!openDevice(mode))
        return false;

    m_mode = mode;
    m_map = nullptr;

    return true;
}

void IO::close(void)
{
    VOLCANO_ASSERT(isOpen());

    closeDevice();

    m_mode = 0;
    m_map = nullptr;
}

bool IO::isOpen(void) const
{
    return bool(m_mode);
}

int IO::mode(void) const
{
    VOLCANO_ASSERT(isOpen());

    return m_mode;
}

bool IO::isEof(void)
{
    VOLCANO_ASSERT(isOpen());
    VOLCANO_ASSERT(deviceFlags() & FlagSeekable);

    return (devicePos() == deviceSize());
}

int64_t IO::tell(void)
{
    VOLCANO_ASSERT(isOpen());
    VOLCANO_ASSERT(deviceFlags() & FlagSeekable);

    return devicePos();
}

int64_t IO::seek(int64_t offset, SeekMode seekMode)
{
    VOLCANO_ASSERT(isOpen());
    VOLCANO_ASSERT(flags() & FlagSeekable);

    int64_t size = deviceSize();
    int64_t curPos = devicePos();
    int64_t newPos;

    switch (seekMode) {
    case SeekMode::Set:
        newPos = offset;
        break;
    case SeekMode::Cur:
        newPos = curPos + offset;
        break;
    case SeekMode::End:
        newPos = size + offset;
        break;
    default:
        return -1;
    }

    if (!setDevicePos(newPos))
        return -1;

    return devicePos();
}

int64_t IO::read(void *buf, int64_t len)
{
    VOLCANO_ASSERT(isOpen());
    VOLCANO_ASSERT(m_mode & ModeRead);
    VOLCANO_ASSERT(len > 0);

    return readDevice(buf, len);
}

int64_t IO::write(const void *buf, int64_t len)
{
    VOLCANO_ASSERT(flags() & FlagWritable);
    VOLCANO_ASSERT(isOpen());
    VOLCANO_ASSERT(len > 0);

    return writeDevice(buf, len);
}

void *IO::map(void)
{
    VOLCANO_ASSERT(isOpen());
    VOLCANO_ASSERT(m_map == nullptr);

    m_map = mapDevice();

    return m_map;
}

void IO::unmap(void)
{
    VOLCANO_ASSERT(isOpen());

    if (m_map) {
        unmapDevice();
        m_map = nullptr;
    }
}

VOLCANO_END
