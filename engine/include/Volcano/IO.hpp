//
//
#ifndef VOLCANO_IO_HPP
#define VOLCANO_IO_HPP

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class IO: public Noncopyable {
public:
    enum {
        FLAG_READ = 0x1,
        FLAG_WRITE = 0x2,
        FLAG_CREATE = 0x4,
        FLAG_EXISTED = 0x8,
        FLAG_TRUNCATE = 0x10,

        // internal
        FLAG_SEEKABLE = 0x10000
    };

    enum SeekMode {
        SEEK_MODE_START = 0,
        SEEK_MODE_CURRENT,
        SEEK_MODE_END
    };

public:
    IO(void);
    virtual ~IO(void);

public:
    virtual bool open(int flags);
    virtual void close(void);
    int flags(void) const;
    bool isOpen(void) const;
    bool isReadable(void) const;
    bool isWritable(void) const;
    bool isSeekable(void) const;
    virtual int64_t size(void) = 0;
    int64_t tell(void);
    int64_t seek(int64_t offset, SeekMode mode = SEEK_MODE_START);
    virtual int64_t read(void *p, int64_t size) = 0;
    virtual int64_t write(const void *p, int64_t size) = 0;

protected:
    virtual int64_t pos(void) = 0;
    virtual int64_t setPos(int64_t v) = 0;
    void setSeekable(bool v);

private:
    int m_flags;
};

VOLCANO_INLINE int IO::flags(void) const
{
    VOLCANO_ASSERT(isOpen());

    return m_flags;
}

VOLCANO_INLINE bool IO::isOpen(void) const
{
    return (m_flags != 0);
}

VOLCANO_INLINE bool IO::isReadable(void) const
{
    VOLCANO_ASSERT(isOpen());

    return (m_flags & FLAG_READ);
}

VOLCANO_INLINE bool IO::isWritable(void) const
{
    VOLCANO_ASSERT(isOpen());

    return (m_flags & (FLAG_WRITE | FLAG_CREATE | FLAG_TRUNCATE));
}

VOLCANO_INLINE bool IO::isSeekable(void) const
{
    VOLCANO_ASSERT(isOpen());

    return (m_flags & FLAG_SEEKABLE);
}

VOLCANO_INLINE int64_t IO::tell(void)
{
    VOLCANO_ASSERT(isOpen());
    VOLCANO_ASSERT(isSeekable());

    return pos();
}

VOLCANO_END

#endif // VOLCANO_IO_HPP
