//
//
#ifndef VOLCANO_BUFFER_HPP
#define VOLCANO_BUFFER_HPP

#include <Volcano/Common.hpp>
#include <Volcano/IO.hpp>

VOLCANO_BEGIN

class Buffer: public IO {
public:
    Buffer(void);
    Buffer(size_t size);
    Buffer(const ByteArray &byteArray);
    Buffer(const Buffer &that);
    ~Buffer(void) override;

public:
    int flags(void) override;
    int64_t size(void) override;
    bool open(int mode) override;
    void close(void) override;
    bool isEof(void) override;
    const ByteArray &toByteArray(void) const;
    Buffer &operator=(const Buffer &that);

protected:
    int64_t pos(void) override;
    bool setPos(int64_t pos) override;
    int64_t readData(void *buf, int64_t len) override;
    int64_t writeData(const void *buf, int64_t len) override;

private:
    ByteArray m_byteArray;
    int64_t m_pos;
};

VOLCANO_END

#endif // VOLCANO_BUFFER_HPP
