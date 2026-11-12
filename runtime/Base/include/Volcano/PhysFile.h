#ifndef VOLCANO_PHYSFILE_H
#define VOLCANO_PHYSFILE_H

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <span>
#include <vector>

#include <google/protobuf/io/zero_copy_stream.h>
#include <physfs.h>

#include <Volcano/ByteArray.h>
#include <Volcano/Common.h>

VOLCANO_BEGIN

class PhysInputStream final : public google::protobuf::io::ZeroCopyInputStream {
public:
    static constexpr int DefaultBufferSize = 64 * 1024;

    explicit PhysInputStream(const std::filesystem::path& path, int bufferSize = DefaultBufferSize);
    explicit PhysInputStream(PHYSFS_File* file, bool ownsHandle = true, int bufferSize = DefaultBufferSize);
    ~PhysInputStream() override;

    PhysInputStream(const PhysInputStream&) = delete;
    PhysInputStream& operator=(const PhysInputStream&) = delete;
    PhysInputStream(PhysInputStream&& other) noexcept;
    PhysInputStream& operator=(PhysInputStream&& other) noexcept;

    bool Next(const void** data, int* size) override;
    void BackUp(int count) override;
    bool Skip(int count) override;
    int64_t ByteCount() const override;

    PHYSFS_File* handle() const noexcept {
        return file_;
    }

    int64_t fileLength() const;
    ByteArray readAll();

private:
    PHYSFS_File* file_ = nullptr;
    bool owns_handle_ = true;
    std::vector<uint8_t> buffer_;
    int buffer_size_ = 0;
    int buffer_offset_ = 0;
    int64_t total_bytes_read_ = 0;
};

class PhysOutputStream final : public google::protobuf::io::ZeroCopyOutputStream {
public:
    static constexpr int DefaultBufferSize = 64 * 1024;

    explicit PhysOutputStream(const std::filesystem::path& path, int bufferSize = DefaultBufferSize);
    explicit PhysOutputStream(PHYSFS_File* file, bool ownsHandle = true, int bufferSize = DefaultBufferSize);
    ~PhysOutputStream() override;

    PhysOutputStream(const PhysOutputStream&) = delete;
    PhysOutputStream& operator=(const PhysOutputStream&) = delete;
    PhysOutputStream(PhysOutputStream&& other) noexcept;
    PhysOutputStream& operator=(PhysOutputStream&& other) noexcept;

    bool Next(void** data, int* size) override;
    void BackUp(int count) override;
    int64_t ByteCount() const override;
    bool Flush();
    bool writeAll(std::span<const uint8_t> data);

    PHYSFS_File* handle() const noexcept {
        return file_;
    }

private:
    bool writeBuffer();

private:
    PHYSFS_File* file_ = nullptr;
    bool owns_handle_ = true;
    std::vector<uint8_t> buffer_;
    int buffer_offset_ = 0;
    int64_t total_bytes_written_ = 0;
};

using PhysFile = PhysInputStream;

VOLCANO_END

#endif // VOLCANO_PHYSFILE_H
