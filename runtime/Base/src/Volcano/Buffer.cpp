#include <algorithm>
#include <cstring>
#include <functional>
#include <utility>

#include <Volcano/Buffer.h>

VOLCANO_BEGIN

Buffer::Buffer(ByteArray data) : data_(std::move(data)) {
}

void Buffer::open(OpenMode mode) {
    VOLCANO_ASSERT(!isOpen());
    if (mode == NotOpen || (mode & ~(ReadWrite | Append | Truncate)) != 0) {
        throw std::invalid_argument("Invalid Buffer open mode");
    }
    if ((mode & (Append | Truncate)) != 0) {
        mode = mode | WriteOnly;
    }
    if ((mode & Truncate) != 0) {
        data_.clear();
    }

    position_ = (mode & Append) != 0 ? data_.size() : 0;
    mode_ = mode;
}

void Buffer::close() noexcept {
    mode_ = NotOpen;
    position_ = 0;
}

void Buffer::setData(ByteArray data) {
    VOLCANO_ASSERT(!isOpen());

    data_ = std::move(data);
}

std::size_t Buffer::read(std::span<std::uint8_t> data) {
    if (data.empty()) {
        return 0;
    }
    VOLCANO_ASSERT(isReadable());
    if (position_ >= data_.size()) {
        return 0;
    }

    const auto count = std::min(data.size(), data_.size() - position_);
    std::memmove(data.data(), data_.data() + position_, count);
    position_ += count;
    return count;
}

std::size_t Buffer::write(std::span<const std::uint8_t> data) {
    if (data.empty()) {
        return 0;
    }
    VOLCANO_ASSERT(isWritable());

    const auto position = (mode_ & Append) != 0 ? data_.size() : position_;
    if (position > data_.max_size() || data.size() > data_.max_size() - position) {
        throw std::length_error("Buffer size exceeds ByteArray capacity");
    }

    const auto end = position + data.size();
    const std::less<const std::uint8_t *> less;
    const bool internal = !data_.empty() && !less(data.data(), data_.data()) &&
        less(data.data(), data_.data() + data_.size());
    // Preserve self-referencing input when growing the ByteArray reallocates it.
    const auto offset = internal ? static_cast<std::size_t>(data.data() - data_.data()) : 0;
    if (end > data_.size()) {
        data_.resize(end);
    }

    std::memmove(data_.data() + position, internal ? data_.data() + offset : data.data(), data.size());
    position_ = end;
    return data.size();
}

void Buffer::seek(std::size_t position) {
    VOLCANO_ASSERT(isOpen());
    if (position > data_.size()) {
        if (!isWritable()) {
            throw std::out_of_range("Seek position is past end of read-only Buffer");
        }

        data_.resize(position);
    }

    position_ = position;
}

VOLCANO_END
