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
    Buffer(const Buffer &that);
    Buffer(Buffer &&that);
    ~Buffer(void) override;

public:
    int deviceFlags(void) override;
    int64_t deviceSize(void) override;
    bool openDevice(int mode) override;
    void closeDevice(void) override;
    int64_t devicePos(void) override;
    bool setDevicePos(int64_t pos) override;
    int64_t readDevice(void *buf, int64_t len) override;
    int64_t writeDevice(const void *buf, int64_t len) override;
    void *mapDevice(void) override;
    void unmapDevice(void) override;

private:
    ByteArray m_byteArray;
    int64_t m_pos;
};

VOLCANO_END

#endif // VOLCANO_BUFFER_HPP
