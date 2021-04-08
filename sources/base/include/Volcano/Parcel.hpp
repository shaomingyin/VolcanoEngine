//
//
#ifndef VOLCANO_PARCEL_HPP
#define VOLCANO_PARCEL_HPP

#include <string>
#include <string_view>
#include <type_traits>

#include <Volcano/Common.hpp>
#include <Volcano/IO.hpp>

VOLCANO_BEGIN

class Parcel {
public:
    Parcel(ByteOrder byteOrder = ByteOrder::BigEndian);
    Parcel(IO *io, ByteOrder byteOrder = ByteOrder::BigEndian);
    virtual ~Parcel(void);

public:
    ByteOrder byteOrder(void) const;
    void setByteOrder(ByteOrder order);
    IO *io(void);
    void setIO(IO *io);

public:
    bool read(bool &r);
    bool write(bool v);

    bool read(int8_t &r);
    bool write(int8_t v);

    bool read(uint8_t &r);
    bool write(uint8_t v);

    bool read(int16_t &r);
    bool write(int16_t v);

    bool read(uint16_t &r);
    bool write(uint16_t v);

    bool read(int32_t &r);
    bool write(int32_t v);

    bool read(uint32_t &r);
    bool write(uint32_t v);

    bool read(int64_t &r);
    bool write(int64_t v);

    bool read(uint64_t &r);
    bool write(uint64_t v);

    bool read(float &r);
    bool write(float v);

    bool read(double &r);
    bool write(double v);

    bool read(std::string &r);
    bool write(std::string_view v);

    bool read(void *buf, size_t len);
    bool write(const void *buf, size_t len);

protected:
    template <typename T>
    bool readMultiBytesInteger(T &r)
    {
        static_assert(std::is_integral<T>::value);

        VOLCANO_ASSERT(m_io != nullptr);

        int64_t ret;
        uint8_t buf[sizeof(T)];

        ret = m_io->read(buf, sizeof(T));
        if (ret != sizeof(T))
            return false;

        switch (m_byteOrder) {
        case ByteOrder::BigEndian:
#ifdef VOLCANO_BIG_ENDIAN
            memcpy(buf, &v, sizeof(T));
#else
            memcpyReverse(&r, buf, sizeof(T));
#endif
            break;
        case ByteOrder::LittleEndian:
#ifdef VOLCANO_BIG_ENDIAN
            memcpyReverse(buf, &v, sizeof(T));
#else
            memcpy(&r, buf, sizeof(T));
#endif
            break;
        }

        return true;
    }

    template <typename T>
    bool writeMultiBytesInteger(T v)
    {
        static_assert(std::is_integral<T>::value);

        VOLCANO_ASSERT(m_io != nullptr);

        int64_t ret;
        uint8_t buf[sizeof(T)];

        switch (m_byteOrder) {
        case ByteOrder::BigEndian:
#ifdef VOLCANO_BIG_ENDIAN
            memcpy(buf, &v, sizeof(T));
#else
            memcpyReverse(buf, &v, sizeof(T));
#endif
            break;
        case ByteOrder::LittleEndian:
#ifdef VOLCANO_BIG_ENDIAN
            memcpyReverse(buf, &v, sizeof(T));
#else
            memcpy(buf, &v, sizeof(T));
#endif
            break;
        }

        ret = m_io->write(buf, sizeof(T));

        return (ret == sizeof(T));
    }

private:
    ByteOrder m_byteOrder;
    IO *m_io;
};

VOLCANO_END

#endif // VOLCANO_PARCEL_HPP
