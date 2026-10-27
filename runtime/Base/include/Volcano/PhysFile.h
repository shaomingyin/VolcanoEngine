#ifndef VOLCANO_PHYSFILE_H
#define VOLCANO_PHYSFILE_H

#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <span>

#include <physfs.h>

#include <Volcano/Common.h>
#include <Volcano/Stream.h>

VOLCANO_BEGIN

// Owns a PhysFS handle; it must be destroyed before PHYSFS_deinit().
class PhysFile final: public Stream {
public:
    enum class OpenMode {
        ReadOnly,
        WriteOnly,
        Append
    };

    PhysFile() noexcept = default;
    explicit PhysFile(const std::filesystem::path& path, OpenMode mode = OpenMode::ReadOnly);
    explicit PhysFile(PHYSFS_File* file, OpenMode mode) noexcept;
    ~PhysFile() override;
    PhysFile(const PhysFile&) = delete;
    PhysFile& operator=(const PhysFile&) = delete;
    PhysFile(PhysFile&& other) noexcept;
    PhysFile& operator=(PhysFile&& other);

    bool isOpen() const noexcept;
    bool isReadable() const override;
    bool isWritable() const override;
    bool isSequential() const override;
    PHYSFS_File* get() const noexcept;

    using Stream::read;
    using Stream::write;

    std::size_t read(std::span<std::uint8_t> data) override;
    std::size_t write(std::span<const std::uint8_t> data) override;
    void flush() override;
    void seek(std::size_t position) override;
    std::size_t tell() const override;
    std::size_t size() override;
    bool eof() const;

    // Reports close/flush errors; on failure the handle remains owned and open.
    void close();

private:
    void requireOpen() const;

private:
    PHYSFS_File* file_ = nullptr;
    OpenMode mode_ = OpenMode::ReadOnly;
};

VOLCANO_END

#endif // VOLCANO_PHYSFILE_H
