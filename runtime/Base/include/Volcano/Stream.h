#ifndef VOLCANO_STREAM_H
#define VOLCANO_STREAM_H

#include <cstddef>
#include <cstdint>
#include <span>
#include <stdexcept>

#include <Volcano/Common.h>
#include <Volcano/ByteArray.h>

VOLCANO_BEGIN

// Synchronous byte stream. Concurrent access to one stream requires caller synchronization.
class Stream {
public:
    virtual ~Stream() = default;

    virtual bool isReadable() const = 0;
    virtual bool isWritable() const = 0;
    virtual bool isSequential() const;

    // Short transfers are allowed. For a nonempty buffer, read returning zero means EOF.
    // Empty buffers return zero. Errors throw; a transfer may have partially completed.
    virtual std::size_t read(std::span<std::uint8_t> data);
    virtual std::size_t write(std::span<const std::uint8_t> data);
    // Flush implementation buffers; this does not guarantee durable storage.
    virtual void flush();

    // Unsupported positioning and size queries throw.
    virtual void seek(std::size_t position);
    virtual std::size_t tell() const;
    virtual std::size_t size();

    void readExact(std::span<std::uint8_t> data);
    void writeAll(std::span<const std::uint8_t> data);
    // Read from the current position to EOF without seeking or querying size.
    ByteArray readAll();

    std::size_t read(void *data, std::size_t size);
    std::size_t write(const void *data, std::size_t size);
};

VOLCANO_END

#endif // VOLCANO_STREAM_H
