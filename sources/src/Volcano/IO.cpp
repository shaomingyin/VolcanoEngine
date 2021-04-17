//
//
#include <algorithm>

#include <Volcano/IO.hpp>

VOLCANO_BEGIN

IO::IO(void):
    m_mode(0)
{
}

IO::~IO(void)
{
}

bool IO::open(int mode)
{
    VOLCANO_ASSERT(!isOpen());

    VOLCANO_ASSERT(mode != 0);

    if (mode & (ModeWrite | ModeCreate | ModeAppend | ModeTruncate)) {
        if (!(flags() & FlagWritable))
            return false;
    }

    if (mode & ModeRead) {
        if (!(flags() & FlagReadable))
            return false;
    }

    m_mode = mode;

    return true;
}

void IO::close(void)
{
    VOLCANO_ASSERT(isOpen());

    m_mode = 0;
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

int64_t IO::tell(void)
{
    VOLCANO_ASSERT(isOpen());
    VOLCANO_ASSERT(flags() & FlagSeekable);

    return pos();
}

int64_t IO::seek(int64_t offset, SeekMode seekMode)
{
    VOLCANO_ASSERT(isOpen());
    VOLCANO_ASSERT(flags() & FlagSeekable);

    int64_t siz = size();
    int64_t curPos = pos();
    int64_t newPos;

    switch (seekMode) {
    case SeekMode::Set:
        newPos = offset;
        break;
    case SeekMode::Cur:
        newPos = curPos + offset;
        break;
    case SeekMode::End:
        newPos = siz + offset;
        break;
    default:
        return -1;
    }

    if (!setPos(newPos))
        return -1;

    return newPos;
}

int64_t IO::read(void *buf, int64_t len)
{
    VOLCANO_ASSERT(isOpen());
    VOLCANO_ASSERT(m_mode & ModeRead);
    VOLCANO_ASSERT(len > 0);

    return readData(buf, len);
}

int64_t IO::write(const void *buf, int64_t len)
{
    VOLCANO_ASSERT(flags() & FlagWritable);
    VOLCANO_ASSERT(isOpen());
    VOLCANO_ASSERT(buf != nullptr);
    VOLCANO_ASSERT(len > 0);

    return writeData(buf, len);
}

VOLCANO_END
