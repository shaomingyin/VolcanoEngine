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

    bool isOpen() const noexcept {
        return mode_ != NotOpen;
    }

    bool isReadable() const override {
        return (mode_ & ReadOnly) != 0;
    }

    bool isWritable() const override {
        return (mode_ & WriteOnly) != 0;
    }

    bool isSequential() const override {
        return false;
    }

    ByteArray &buffer() noexcept {
        return data_;
    }

    const ByteArray &buffer() const noexcept {
        return data_;
    }

    const ByteArray &data() const noexcept {
        return data_;
    }

    void setData(ByteArray data);

    using Stream::read;
    using Stream::write;

    std::size_t read(std::span<std::uint8_t> data) override;
    std::size_t write(std::span<const std::uint8_t> data) override;
    void seek(std::size_t position) override;

    std::size_t tell() const override {
        return position_;
    }

    std::size_t size() override {
        return data_.size();
    }

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
