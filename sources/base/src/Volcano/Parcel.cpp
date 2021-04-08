//
//
#include <Volcano/DataStream.hpp>

VOLCANO_BEGIN

DataStream::DataStream(ByteOrder byteOrder):
    m_byteOrder(byteOrder),
    m_io(nullptr)
{
}

DataStream::DataStream(IO *io, ByteOrder byteOrder):
    m_byteOrder(byteOrder),
    m_io(io)
{
}

DataStream::~DataStream(void)
{
}

ByteOrder DataStream::byteOrder(void) const
{
    return m_byteOrder;
}

void DataStream::setByteOrder(ByteOrder order)
{
    m_byteOrder = order;
}

IO *DataStream::io(void)
{
    return m_io;
}

void DataStream::setIO(IO *io)
{
    VOLCANO_ASSERT(m_io == nullptr);

    m_io = io;
}

bool DataStream::read(bool &r)
{
    VOLCANO_ASSERT(m_io != nullptr);

    auto ret = m_io->read(&r, sizeof(bool));

    return (ret == sizeof(bool));
}

bool DataStream::write(bool v)
{
    VOLCANO_ASSERT(m_io != nullptr);

    auto ret = m_io->write(&v, sizeof(v));

    return (ret == sizeof(v));
}

bool DataStream::read(int8_t &r)
{
    VOLCANO_ASSERT(m_io != nullptr);

    auto ret = m_io->read(&r, sizeof(int8_t));

    return (ret == sizeof(int8_t));
}

bool DataStream::write(int8_t v)
{
    VOLCANO_ASSERT(m_io != nullptr);

    auto ret = m_io->write(&v, sizeof(v));

    return (ret == sizeof(v));
}

bool DataStream::write(int16_t v)
{
    return writeTemplate(v);
}

bool DataStream::write(int32_t v)
{
    return writeTemplate(v);
}

bool DataStream::write(int64_t v)
{
    return writeTemplate(v);
}

bool DataStream::write(float v)
{
    VOLCANO_ASSERT(m_io != nullptr);

    int64_t ret = m_io->write(&v, sizeof(float));

    return (ret == sizeof(float));
}

bool DataStream::write(double v)
{
    VOLCANO_ASSERT(m_io != nullptr);

    int64_t ret = m_io->write(&v, sizeof(float));

    return (ret == sizeof(float));
}

bool DataStream::write(std::string_view v)
{
    VOLCANO_ASSERT(m_io != nullptr);

    if (write(int32_t(v.size())))
        return m_io->write(v.data(), v.size());

    return false;
}

VOLCANO_END
