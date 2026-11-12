#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include <google/protobuf/io/zero_copy_stream.h>
#include <physfs.h>

#include <Volcano/Error.h>
#include <Volcano/PhysFile.h>

VOLCANO_BEGIN

PhysInputStream::PhysInputStream(const std::filesystem::path& path, int bufferSize)
    : file_(PHYSFS_openRead(path.generic_string().c_str())),
      owns_handle_(true),
      buffer_(bufferSize > 0 ? bufferSize : DefaultBufferSize) {
    if (file_ == nullptr) {
        throw std::runtime_error("Cannot open PhysFS file for reading: " + path.generic_string() + ": " +
                                 PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    }
}

PhysInputStream::PhysInputStream(PHYSFS_File* file, bool ownsHandle, int bufferSize)
    : file_(file),
      owns_handle_(ownsHandle),
      buffer_(bufferSize > 0 ? bufferSize : DefaultBufferSize) {
    if (file_ == nullptr) {
        throw std::invalid_argument("PHYSFS_File handle cannot be null");
    }
}

PhysInputStream::~PhysInputStream() {
    if (file_ != nullptr && owns_handle_) {
        PHYSFS_close(file_);
    }
}

PhysInputStream::PhysInputStream(PhysInputStream&& other) noexcept
    : file_(std::exchange(other.file_, nullptr)),
      owns_handle_(std::exchange(other.owns_handle_, false)),
      buffer_(std::move(other.buffer_)),
      buffer_size_(std::exchange(other.buffer_size_, 0)),
      buffer_offset_(std::exchange(other.buffer_offset_, 0)),
      total_bytes_read_(std::exchange(other.total_bytes_read_, 0)) {}

PhysInputStream& PhysInputStream::operator=(PhysInputStream&& other) noexcept {
    if (this != &other) {
        if (file_ != nullptr && owns_handle_) {
            PHYSFS_close(file_);
        }
        file_ = std::exchange(other.file_, nullptr);
        owns_handle_ = std::exchange(other.owns_handle_, false);
        buffer_ = std::move(other.buffer_);
        buffer_size_ = std::exchange(other.buffer_size_, 0);
        buffer_offset_ = std::exchange(other.buffer_offset_, 0);
        total_bytes_read_ = std::exchange(other.total_bytes_read_, 0);
    }
    return *this;
}

bool PhysInputStream::Next(const void** data, int* size) {
    if (file_ == nullptr) {
        return false;
    }
    if (buffer_offset_ < buffer_size_) {
        *data = buffer_.data() + buffer_offset_;
        *size = buffer_size_ - buffer_offset_;
        total_bytes_read_ += *size;
        buffer_offset_ = buffer_size_;
        return true;
    }
    if (PHYSFS_eof(file_) != 0) {
        return false;
    }
    const auto bytesRead = PHYSFS_readBytes(file_, buffer_.data(), buffer_.size());
    if (bytesRead <= 0) {
        return false;
    }
    buffer_size_ = static_cast<int>(bytesRead);
    buffer_offset_ = buffer_size_;
    *data = buffer_.data();
    *size = buffer_size_;
    total_bytes_read_ += buffer_size_;
    return true;
}

void PhysInputStream::BackUp(int count) {
    VOLCANO_ASSERT(count >= 0 && count <= buffer_offset_);
    buffer_offset_ -= count;
    total_bytes_read_ -= count;
}

bool PhysInputStream::Skip(int count) {
    if (count <= 0) {
        return count == 0;
    }
    const int available = buffer_size_ - buffer_offset_;
    if (count <= available) {
        buffer_offset_ += count;
        total_bytes_read_ += count;
        return true;
    }
    count -= available;
    total_bytes_read_ += available;
    buffer_offset_ = buffer_size_;
    const auto currentPos = PHYSFS_tell(file_);
    if (currentPos >= 0) {
        if (PHYSFS_seek(file_, currentPos + count) != 0) {
            total_bytes_read_ += count;
            return true;
        }
    }
    return false;
}

int64_t PhysInputStream::ByteCount() const {
    return total_bytes_read_;
}

int64_t PhysInputStream::fileLength() const {
    if (file_ == nullptr) {
        return -1;
    }
    return PHYSFS_fileLength(file_);
}

ByteArray PhysInputStream::readAll() {
    ByteArray result;
    const void* data = nullptr;
    int size = 0;
    while (Next(&data, &size)) {
        if (size > 0) {
            result.insert(result.end(), static_cast<const uint8_t*>(data),
                          static_cast<const uint8_t*>(data) + size);
        }
    }
    return result;
}

PhysOutputStream::PhysOutputStream(const std::filesystem::path& path, int bufferSize)
    : file_(PHYSFS_openWrite(path.generic_string().c_str())),
      owns_handle_(true),
      buffer_(bufferSize > 0 ? bufferSize : DefaultBufferSize) {
    if (file_ == nullptr) {
        throw std::runtime_error("Cannot open PhysFS file for writing: " + path.generic_string() + ": " +
                                 PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    }
}

PhysOutputStream::PhysOutputStream(PHYSFS_File* file, bool ownsHandle, int bufferSize)
    : file_(file),
      owns_handle_(ownsHandle),
      buffer_(bufferSize > 0 ? bufferSize : DefaultBufferSize) {
    if (file_ == nullptr) {
        throw std::invalid_argument("PHYSFS_File handle cannot be null");
    }
}

PhysOutputStream::~PhysOutputStream() {
    Flush();
    if (file_ != nullptr && owns_handle_) {
        PHYSFS_close(file_);
    }
}

PhysOutputStream::PhysOutputStream(PhysOutputStream&& other) noexcept
    : file_(std::exchange(other.file_, nullptr)),
      owns_handle_(std::exchange(other.owns_handle_, false)),
      buffer_(std::move(other.buffer_)),
      buffer_offset_(std::exchange(other.buffer_offset_, 0)),
      total_bytes_written_(std::exchange(other.total_bytes_written_, 0)) {}

PhysOutputStream& PhysOutputStream::operator=(PhysOutputStream&& other) noexcept {
    if (this != &other) {
        Flush();
        if (file_ != nullptr && owns_handle_) {
            PHYSFS_close(file_);
        }
        file_ = std::exchange(other.file_, nullptr);
        owns_handle_ = std::exchange(other.owns_handle_, false);
        buffer_ = std::move(other.buffer_);
        buffer_offset_ = std::exchange(other.buffer_offset_, 0);
        total_bytes_written_ = std::exchange(other.total_bytes_written_, 0);
    }
    return *this;
}

bool PhysOutputStream::Next(void** data, int* size) {
    if (file_ == nullptr) {
        return false;
    }
    if (buffer_offset_ == static_cast<int>(buffer_.size())) {
        if (!writeBuffer()) {
            return false;
        }
    }
    *data = buffer_.data() + buffer_offset_;
    *size = static_cast<int>(buffer_.size()) - buffer_offset_;
    total_bytes_written_ += *size;
    buffer_offset_ = static_cast<int>(buffer_.size());
    return true;
}

void PhysOutputStream::BackUp(int count) {
    VOLCANO_ASSERT(count >= 0 && count <= buffer_offset_);
    buffer_offset_ -= count;
    total_bytes_written_ -= count;
}

int64_t PhysOutputStream::ByteCount() const {
    return total_bytes_written_;
}

bool PhysOutputStream::writeBuffer() {
    if (file_ == nullptr || buffer_offset_ == 0) {
        return true;
    }
    const auto written = PHYSFS_writeBytes(file_, buffer_.data(), buffer_offset_);
    if (written != buffer_offset_) {
        return false;
    }
    buffer_offset_ = 0;
    return true;
}

bool PhysOutputStream::Flush() {
    if (!writeBuffer()) {
        return false;
    }
    return PHYSFS_flush(file_) != 0;
}

bool PhysOutputStream::writeAll(std::span<const uint8_t> data) {
    const auto* in = data.data();
    int count = static_cast<int>(data.size());
    while (count > 0) {
        void* chunk = nullptr;
        int chunkSize = 0;
        if (!Next(&chunk, &chunkSize)) {
            return false;
        }
        const int toCopy = std::min(count, chunkSize);
        std::memcpy(chunk, in, toCopy);
        in += toCopy;
        count -= toCopy;
        if (toCopy < chunkSize) {
            BackUp(chunkSize - toCopy);
        }
    }
    return Flush();
}

VOLCANO_END
