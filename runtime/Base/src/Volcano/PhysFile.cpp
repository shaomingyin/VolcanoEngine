#include <stdexcept>
#include <string>
#include <utility>

#include <Volcano/PhysFile.h>

VOLCANO_BEGIN

PhysFile::PhysFile(const std::filesystem::path& path, OpenMode mode)
    : mode_(mode) {
    const auto utf8 = path.generic_u8string();
    const std::string filename(utf8.begin(), utf8.end());
    if (filename.find('\0') != std::string::npos) {
        throw std::invalid_argument("PhysFS file path must not contain NUL");
    }

    switch (mode_) {
    case OpenMode::ReadOnly:
        file_ = PHYSFS_openRead(filename.c_str());
        break;
    case OpenMode::WriteOnly:
        file_ = PHYSFS_openWrite(filename.c_str());
        break;
    case OpenMode::Append:
        file_ = PHYSFS_openAppend(filename.c_str());
        break;
    default:
        throw std::invalid_argument("Invalid PhysFile open mode");
    }
    if (file_ == nullptr) {
        throw std::runtime_error("Cannot open PhysFS file '" + filename + "': " +
            PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    }
}

PhysFile::PhysFile(PHYSFS_File* file, OpenMode mode) noexcept
    : file_(file), mode_(mode) {
}

PhysFile::~PhysFile() {
    if (file_ != nullptr) {
        // Destruction cannot report flush errors; writers should call close() explicitly.
        PHYSFS_close(file_);
    }
}

PhysFile::PhysFile(PhysFile&& other) noexcept
    : file_(std::exchange(other.file_, nullptr)), mode_(other.mode_) {
}

PhysFile& PhysFile::operator=(PhysFile&& other) {
    if (this != &other) {
        close();
        file_ = std::exchange(other.file_, nullptr);
        mode_ = other.mode_;
    }

    return *this;
}

PHYSFS_File* PhysFile::get() const noexcept {
    return file_;
}

bool PhysFile::isOpen() const noexcept {
    return file_ != nullptr;
}

bool PhysFile::isReadable() const {
    return isOpen() && mode_ == OpenMode::ReadOnly;
}

bool PhysFile::isWritable() const {
    return isOpen() && (mode_ == OpenMode::WriteOnly || mode_ == OpenMode::Append);
}

bool PhysFile::isSequential() const {
    return false;
}

void PhysFile::requireOpen() const {
    if (!isOpen()) {
        throw std::logic_error("PhysFS file is not open");
    }
}

std::size_t PhysFile::read(std::span<std::uint8_t> data) {
    if (data.empty()) {
        return 0;
    }
    requireOpen();
    if (!isReadable()) {
        throw std::runtime_error("PhysFS file is not readable");
    }
    if (!std::in_range<PHYSFS_sint64>(data.size())) {
        throw std::length_error("PhysFS read size exceeds its supported range");
    }

    const auto count = PHYSFS_readBytes(file_, data.data(), data.size());
    if (count < 0) {
        throw std::runtime_error(std::string("Cannot read PhysFS file: ") +
            PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    }

    return static_cast<std::size_t>(count);
}

std::size_t PhysFile::write(std::span<const std::uint8_t> data) {
    if (data.empty()) {
        return 0;
    }
    requireOpen();
    if (!isWritable()) {
        throw std::runtime_error("PhysFS file is not writable");
    }
    if (!std::in_range<PHYSFS_sint64>(data.size())) {
        throw std::length_error("PhysFS write size exceeds its supported range");
    }

    const auto count = PHYSFS_writeBytes(file_, data.data(), data.size());
    if (count < 0) {
        throw std::runtime_error(std::string("Cannot write PhysFS file: ") +
            PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    }

    return static_cast<std::size_t>(count);
}

void PhysFile::flush() {
    requireOpen();
    if (PHYSFS_flush(file_) == 0) {
        throw std::runtime_error(std::string("Cannot flush PhysFS file: ") +
            PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    }
}

void PhysFile::seek(std::size_t position) {
    requireOpen();
    if (PHYSFS_seek(file_, position) == 0) {
        throw std::runtime_error(std::string("Cannot seek PhysFS file: ") +
            PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    }
}

std::size_t PhysFile::tell() const {
    requireOpen();
    const auto position = PHYSFS_tell(file_);
    if (position < 0) {
        throw std::runtime_error(std::string("Cannot query PhysFS file position: ") +
            PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    }
    if (!std::in_range<std::size_t>(position)) {
        throw std::overflow_error("PhysFS file position exceeds size_t");
    }

    return static_cast<std::size_t>(position);
}

std::size_t PhysFile::size() {
    requireOpen();
    const auto length = PHYSFS_fileLength(file_);
    if (length < 0) {
        throw std::runtime_error(std::string("Cannot query PhysFS file size: ") +
            PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    }
    if (!std::in_range<std::size_t>(length)) {
        throw std::overflow_error("PhysFS file size exceeds size_t");
    }

    return static_cast<std::size_t>(length);
}

bool PhysFile::eof() const {
    requireOpen();
    return PHYSFS_eof(file_) != 0;
}

void PhysFile::close() {
    if (file_ == nullptr) {
        return;
    }
    if (PHYSFS_close(file_) == 0) {
        throw std::runtime_error(std::string("Cannot close PhysFS file: ") +
            PHYSFS_getErrorByCode(PHYSFS_getLastErrorCode()));
    }

    file_ = nullptr;
}

VOLCANO_END
