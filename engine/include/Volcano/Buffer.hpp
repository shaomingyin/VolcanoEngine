//
//
#ifndef VOLCANO_BUFFER_HPP
#define VOLCANO_BUFFER_HPP

#include <vector>

#include <Volcano/Common.hpp>
#include <Volcano/IO.hpp>

VOLCANO_BEGIN

class Buffer: public IO {
public:
    Buffer(void);
    Buffer(const Buffer &that);
    ~Buffer(void) override;

public:
    bool open(int flags) override;
    int64_t size(void) override;
    int64_t read(void *p, int64_t len) override;
    int64_t write(const void *p, int64_t len) override;

public:
    bool operator==(const Buffer &that);
    bool operator!=(const Buffer &that);
    Buffer &operator=(const Buffer &that);

public:
    ByteArray data;

protected:
    int64_t pos(void) override;
    int64_t setPos(int64_t pos) override;

private:
    int64_t m_pos;
};

VOLCANO_INLINE bool Buffer::operator==(const Buffer &that)
{
    return (data == that.data);
}

VOLCANO_INLINE bool Buffer::operator!=(const Buffer &that)
{
    return (data != that.data);
}

VOLCANO_INLINE Buffer &Buffer::operator=(const Buffer &that)
{
    if ((*this) != that)
        data = that.data;
    return (*this);
}

VOLCANO_END

#endif // VOLCANO_BUFFER_HPP
