#ifndef VOLCANO_BUFFER_H
#define VOLCANO_BUFFER_H

#include <Volcano/Stream.h>

VOLCANO_BEGIN

class Buffer final : public Stream {
public:
    enum OpenMode : unsigned {
        NotOpen = 0,
        ReadOnly = 1,
        WriteOnly = 2,
        ReadWrite = ReadOnly | WriteOnly,
        Append = 4,
        Truncate = 8
    };

    Buffer() = default;
    explicit Buffer(ByteArray data);
    Buffer(const Buffer &) = delete;
    Buffer &operator=(const Buffer &) = delete;

    void open(OpenMode mode);
    void close() noexcept;
    bool isOpen() const noexcept;
    bool isReadable() const override;
    bool isWritable() const override;
    bool isSequential() const override;

    ByteArray &buffer() noexcept;
    const ByteArray &buffer() const noexcept;
    const ByteArray &data() const noexcept;
    void setData(ByteArray data);

    using Stream::read;
    using Stream::write;

    std::size_t read(std::span<std::uint8_t> data) override;
    std::size_t write(std::span<const std::uint8_t> data) override;
    void seek(std::size_t position) override;
    std::size_t tell() const override;
    std::size_t size() override;

private:
    ByteArray data_;
    OpenMode mode_ = NotOpen;
    std::size_t position_ = 0;
};

constexpr Buffer::OpenMode operator|(Buffer::OpenMode left, Buffer::OpenMode right) noexcept {
    return static_cast<Buffer::OpenMode>(static_cast<unsigned>(left) | static_cast<unsigned>(right));
}

VOLCANO_END

#endif // VOLCANO_BUFFER_H
