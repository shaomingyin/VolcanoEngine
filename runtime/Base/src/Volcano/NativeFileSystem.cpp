#include <Volcano/NativeFileSystem.h>

#include <algorithm>
#include <fstream>
#include <limits>

VOLCANO_BEGIN
namespace {

namespace fs = std::filesystem;

class NativeFile final: public File {
public:
    NativeFile(const fs::path &path, bool writable, bool append = false)
        : stream_(path, std::ios::binary | (writable ? std::ios::out : std::ios::in) |
              (writable ? (append ? std::ios::app : std::ios::trunc) : std::ios::openmode{})),
          writable_(writable), append_(append) {
        if (!stream_.is_open()) {
            throw std::runtime_error("Cannot open file: " + path.generic_string());
        }
        if (append_) {
            position_ = size();
        }
    }

    bool isReadable() const override {
        return !writable_;
    }

    bool isWritable() const override {
        return writable_;
    }

    std::size_t read(std::span<std::uint8_t> data) override {
        if (data.empty()) {
            return 0;
        }
        if (writable_) {
            throw std::runtime_error("File is write-only");
        }
        std::size_t total = 0;
        while (total < data.size()) {
            const auto count = chunkSize(data.size() - total);
            stream_.read(reinterpret_cast<char *>(data.data() + total), count);
            const auto actual = static_cast<std::size_t>(stream_.gcount());
            total += actual;
            position_ += actual;
            if (stream_.bad() || (stream_.fail() && !stream_.eof())) {
                throw std::runtime_error("Failed to read file");
            }
            if (actual < static_cast<std::size_t>(count)) {
                break;
            }
        }
        return total;
    }

    std::size_t write(std::span<const std::uint8_t> data) override {
        if (data.empty()) {
            return 0;
        }
        if (!writable_) {
            throw std::runtime_error("File is read-only");
        }
        if (append_) {
            position_ = size();
        }
        std::size_t total = 0;
        while (total < data.size()) {
            const auto count = chunkSize(data.size() - total);
            stream_.write(reinterpret_cast<const char *>(data.data() + total), count);
            if (!stream_) {
                throw std::runtime_error("Failed to write file");
            }
            total += static_cast<std::size_t>(count);
            position_ += static_cast<std::size_t>(count);
        }
        return total;
    }

    void seek(std::size_t position) override {
        if (position > static_cast<std::size_t>(std::numeric_limits<std::streamoff>::max()) || position > size()) {
            throw std::out_of_range("Seek position is past end of file");
        }
        stream_.clear();
        if (writable_) {
            stream_.seekp(static_cast<std::streamoff>(position));
        } else {
            stream_.seekg(static_cast<std::streamoff>(position));
        }
        if (!stream_) {
            throw std::runtime_error("Failed to seek file");
        }
        position_ = position;
    }

    std::size_t tell() const override {
        return position_;
    }

    std::size_t size() override {
        stream_.clear();
        std::streampos end;
        if (writable_) {
            stream_.seekp(0, std::ios::end);
            end = stream_.tellp();
            stream_.seekp(static_cast<std::streamoff>(position_));
        } else {
            stream_.seekg(0, std::ios::end);
            end = stream_.tellg();
            stream_.seekg(static_cast<std::streamoff>(position_));
        }
        if (end < std::streampos(0) || !stream_) {
            throw std::runtime_error("Failed to query file size");
        }
        const auto length = static_cast<std::uintmax_t>(static_cast<std::streamoff>(end));
        if (length > std::numeric_limits<std::size_t>::max()) {
            throw std::overflow_error("File is too large");
        }
        return static_cast<std::size_t>(length);
    }

    void flush() override {
        if (writable_) {
            stream_.flush();
            if (!stream_) {
                throw std::runtime_error("Failed to flush file");
            }
        }
    }

private:
    static std::streamsize chunkSize(std::size_t size) {
        return static_cast<std::streamsize>(std::min(size,
            static_cast<std::size_t>(std::numeric_limits<std::streamsize>::max())));
    }

    std::fstream stream_;
    bool writable_;
    bool append_;
    std::size_t position_ = 0;
};

} // namespace

NativeFileSystem::NativeFileSystem(const fs::path &root) : root_(fs::canonical(root)) {
    if (!fs::is_directory(root_)) {
        throw std::invalid_argument("Native filesystem root must be a directory");
    }
}

fs::path NativeFileSystem::resolve(const fs::path &path) const {
    if (path.is_absolute() || path.has_root_path()) {
        throw std::invalid_argument("Local filesystem paths must be relative");
    }
    const auto relative = normalizePath(virtualPath(path));
    auto result = root_;
    // shortcut: validation and opening are separate; use OS directory handles if mounts can be modified by hostile processes.
    for (const auto &part : relative) {
        result /= part;
        std::error_code error;
        const auto status = fs::symlink_status(result, error);
        if (error && !isMissing(error)) {
            throw fs::filesystem_error("Cannot inspect native path", result, error);
        }
        if (fs::is_symlink(status)) {
            throw std::runtime_error("Symbolic links are disabled: " + result.generic_string());
        }
    }
    return result;
}

DirEntry NativeFileSystem::statImpl(const fs::path &path) const {
    const auto native = resolve(path);
    std::error_code error;
    const auto status = fs::status(native, error);
    if (isMissing(error) || (!error && !fs::exists(status))) {
        return DirEntry(path);
    }
    if (error) {
        throw fs::filesystem_error("Cannot stat native path", native, error);
    }
    if (fs::is_directory(status)) {
        return DirEntry(path, FileType::Directory);
    }
    if (!fs::is_regular_file(status)) {
        return DirEntry(path, FileType::Other);
    }
    const auto length = fs::file_size(native);
    if (length > std::numeric_limits<std::size_t>::max()) {
        throw std::overflow_error("File is too large");
    }
    return DirEntry(path, FileType::Regular, static_cast<std::size_t>(length));
}

std::vector<DirEntry> NativeFileSystem::listDirectoryImpl(const fs::path &path) const {
    std::vector<DirEntry> result;
    for (const auto &entry : fs::directory_iterator(resolve(path))) {
        if (fs::is_symlink(entry.symlink_status())) {
            continue;
        }
        const auto info = statImpl(path / entry.path().filename());
        if (info.exists()) {
            result.emplace_back(entry.path().filename(), info.type(), info.isFile() ? info.size() : 0);
        }
    }
    return result;
}

std::unique_ptr<File> NativeFileSystem::openFileForReadImpl(const fs::path &path) const {
    if (!statImpl(path).isFile()) {
        throw std::runtime_error("Cannot read non-regular file: " + path.generic_string());
    }
    return std::make_unique<NativeFile>(resolve(path), false);
}

std::unique_ptr<File> NativeFileSystem::openFileForWriteImpl(const fs::path &path, bool append) const {
    const auto info = statImpl(path);
    if (path.empty() || (info.exists() && !info.isFile())) {
        throw std::runtime_error("Cannot write non-regular file: " + path.generic_string());
    }
    return std::make_unique<NativeFile>(resolve(path), true, append);
}

void NativeFileSystem::removeImpl(const fs::path &path) const {
    if (path.empty()) {
        throw std::invalid_argument("Cannot remove filesystem root");
    }
    if (!fs::remove(resolve(path))) {
        throw std::runtime_error("Cannot remove missing path: " + path.generic_string());
    }
}

void NativeFileSystem::createDirectoryImpl(const fs::path &path) const {
    fs::create_directories(resolve(path));
}

VOLCANO_END
