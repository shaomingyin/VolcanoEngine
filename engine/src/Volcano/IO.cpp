//
//
#include <algorithm>

#include <Volcano/IO.hpp>

VOLCANO_BEGIN

IO::IO(void) :
    m_flags(0)
{
}

IO::~IO(void)
{
}

bool IO::open(int flags)
{
    if (flags == 0)
        return false;

    if (flags & (FLAG_CREATE | FLAG_EXISTED | FLAG_SEEKABLE))
        return false;

    if ((flags & (FLAG_CREATE | FLAG_TRUNCATE)) && !(flags & FLAG_WRITE))
        return false;

    m_flags = flags;

    return true;
}

void IO::close(void)
{
    m_flags = 0;
}

void IO::setSeekable(bool v)
{
    if (v)
        m_flags |= FLAG_SEEKABLE;
    else
        m_flags &= ~FLAG_SEEKABLE;
}

int64_t IO::seek(int64_t offset, SeekMode mode)
{
    VOLCANO_ASSERT(isOpen());
    VOLCANO_ASSERT(isSeekable());

    int64_t newPos;
    int64_t sz = size();

    switch (mode) {
    case SEEK_MODE_START:
        newPos = offset;
        break;
    case SEEK_MODE_CURRENT:
        newPos = pos() + offset;
        break;
    case SEEK_MODE_END:
        newPos = sz + offset;
        break;
    default:
        return -1;
    }

    return setPos(std::clamp(newPos, int64_t(0), sz));
}

VOLCANO_END
