#include <Volcano/Stream.h>

#include <array>

VOLCANO_BEGIN

bool Stream::isSequential() const {
    return true;
}

std::size_t Stream::read(std::span<std::uint8_t> data) {
    if (data.empty()) {
        return 0;
    }
    throw std::runtime_error("Stream is not readable");
}

std::size_t Stream::write(std::span<const std::uint8_t> data) {
    if (data.empty()) {
        return 0;
    }
    throw std::runtime_error("Stream is not writable");
}

void Stream::flush() {
}

void Stream::seek(std::size_t) {
    throw std::runtime_error("Stream does not support seeking");
}

std::size_t Stream::tell() const {
    throw std::runtime_error("Stream does not support position queries");
}

std::size_t Stream::size() {
    throw std::runtime_error("Stream does not support size queries");
}

void Stream::readExact(std::span<std::uint8_t> data) {
    while (!data.empty()) {
        const auto count = read(data);
        if (count == 0 || count > data.size()) {
            throw std::runtime_error("Stream did not supply the requested bytes");
        }
        data = data.subspan(count);
    }
}

void Stream::writeAll(std::span<const std::uint8_t> data) {
    while (!data.empty()) {
        const auto count = write(data);
        if (count == 0 || count > data.size()) {
            throw std::runtime_error("Stream did not accept the requested bytes");
        }
        data = data.subspan(count);
    }
}

ByteArray Stream::readAll() {
    ByteArray result;
    std::array<std::uint8_t, 4096> buffer;
    for (;;) {
        const auto count = read(buffer);
        if (count > buffer.size()) {
            throw std::runtime_error("Stream returned an invalid read count");
        }
        if (count == 0) {
            return result;
        }
        result.insert(result.end(), buffer.begin(), buffer.begin() + count);
    }
}

std::size_t Stream::read(void *data, std::size_t size) {
    if (data == nullptr && size != 0) {
        throw std::invalid_argument("Read buffer is null");
    }
    return read(std::span(static_cast<std::uint8_t *>(data), size));
}

std::size_t Stream::write(const void *data, std::size_t size) {
    if (data == nullptr && size != 0) {
        throw std::invalid_argument("Write buffer is null");
    }
    return write(std::span(static_cast<const std::uint8_t *>(data), size));
}

VOLCANO_END
