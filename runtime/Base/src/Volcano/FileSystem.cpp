#include <Volcano/FileSystem.h>

#include <algorithm>
#include <stdexcept>

VOLCANO_BEGIN

namespace fs = std::filesystem;

std::filesystem::path FileSystem::normalizePath(const std::filesystem::path &path) {
    if (path.has_root_name()) {
        throw std::invalid_argument("Virtual paths cannot contain a drive name");
    }
    if (!path.has_root_directory()) {
        throw std::invalid_argument("Virtual paths must start with '/'");
    }
    std::filesystem::path result;
    for (const auto &part : path.relative_path()) {
        if (part.empty() || part == ".") {
            continue;
        }
        const auto name = part.u8string();
        if (name.find(u8'\\') != name.npos ||
            name.find(u8':') != name.npos || name.find(char8_t{}) != name.npos) {
            throw std::invalid_argument("Virtual paths cannot contain backslashes, colons or null bytes");
        }
        if (part == "..") {
            if (result.empty()) {
                throw std::invalid_argument("Virtual path escapes filesystem root");
            }
            result = result.parent_path();
        } else {
            result /= part;
        }
    }
    return result;
}

bool FileSystem::isMissing(const std::error_code &error) {
    return error == std::errc::no_such_file_or_directory || error == std::errc::not_a_directory;
}

fs::path FileSystem::virtualPath(const fs::path &path) {
    return fs::path("/") / path;
}

bool FileSystem::isPrefix(const fs::path &prefix, const fs::path &path) {
    auto part = path.begin();
    for (const auto &component : prefix) {
        if (part == path.end() || component != *part++) {
            return false;
        }
    }
    return true;
}

fs::path FileSystem::relativeTo(const fs::path &path, const fs::path &prefix) {
    auto part = path.begin();
    for (auto ignored = prefix.begin(); ignored != prefix.end(); ++ignored) {
        ++part;
    }
    fs::path result;
    for (; part != path.end(); ++part) {
        result /= *part;
    }
    return result;
}

std::string FileSystem::toNativeString(const fs::path &path) {
    const auto text = path.u8string();
    return std::string(reinterpret_cast<const char *>(text.data()), text.size());
}

bool FileSystem::isWritable(const fs::path &path) const {
    return isWritableImpl(normalizePath(path));
}

DirEntry FileSystem::stat(const fs::path &path) const {
    const auto relative = normalizePath(path);
    const auto entry = statImpl(relative);
    return DirEntry(virtualPath(relative), entry.type(), entry.isFile() ? entry.size() : 0);
}

bool FileSystem::exists(const fs::path &path) const {
    return stat(path).exists();
}
bool FileSystem::isFile(const fs::path &path) const {
    return stat(path).isFile();
}
bool FileSystem::isDirectory(const fs::path &path) const {
    return stat(path).isDirectory();
}
std::size_t FileSystem::fileSize(const fs::path &path) const {
    return stat(path).size();
}

void FileSystem::requireWritable(const fs::path &path) const {
    if (path.empty()) {
        throw std::invalid_argument("Cannot modify filesystem root");
    }
    if (!isWritableImpl(path)) {
        throw std::runtime_error("Path is read-only or cannot be modified: " + virtualPath(path).generic_string());
    }
}

void FileSystem::remove(const fs::path &path) const {
    const auto relative = normalizePath(path);
    requireWritable(relative);
    removeImpl(relative);
}

void FileSystem::createDirectory(const fs::path &path) const {
    const auto relative = normalizePath(path);
    requireWritable(relative);
    createDirectoryImpl(relative);
}

std::vector<DirEntry> FileSystem::children(const fs::path &path) const {
    if (!statImpl(path).isDirectory()) {
        throw std::runtime_error("Cannot enumerate non-directory: " + virtualPath(path).generic_string());
    }
    std::vector<DirEntry> result;
    for (const auto &entry : listDirectoryImpl(path)) {
        const auto &name = entry.path();
        if (name.empty() || name.has_parent_path() || normalizePath(virtualPath(name)) != name) {
            throw std::runtime_error("Filesystem returned an invalid directory entry");
        }
        if (entry.exists()) {
            result.emplace_back(virtualPath(path / name), entry.type(), entry.isFile() ? entry.size() : 0);
        }
    }
    std::sort(result.begin(), result.end(), [](const DirEntry &left, const DirEntry &right) {
        return left.path() < right.path();
    });
    if (std::adjacent_find(result.begin(), result.end(), [](const DirEntry &left, const DirEntry &right) {
            return left.path() == right.path();
        }) != result.end()) {
        throw std::runtime_error("Filesystem returned duplicate directory entries");
    }
    return result;
}

bool FileSystem::enumerate(const fs::path &path, const DirectoryCallback &callback, bool recursive) const {
    for (const auto &entry : children(path)) {
        if (!callback(entry)) {
            return false;
        }
        if (recursive && entry.isDirectory() && !enumerate(normalizePath(entry.path()), callback, true)) {
            return false;
        }
    }
    return true;
}

void FileSystem::listDirectory(const fs::path &path, const DirectoryCallback &callback, bool recursive) const {
    if (!callback) {
        throw std::invalid_argument("Directory callback is empty");
    }
    enumerate(normalizePath(path), callback, recursive);
}

std::vector<DirEntry> FileSystem::listDirectory(const fs::path &path, bool recursive) const {
    std::vector<DirEntry> result;
    listDirectory(path, [&](const DirEntry &entry) {
        result.push_back(entry);
        return true;
    }, recursive);
    return result;
}

std::unique_ptr<File> FileSystem::openFileForRead(const fs::path &path) const {
    const auto relative = normalizePath(path);
    if (!statImpl(relative).isFile()) {
        throw std::runtime_error("Cannot read non-regular file: " + path.generic_string());
    }
    auto file = openFileForReadImpl(relative);
    if (!file) {
        throw std::runtime_error("Filesystem returned a null file");
    }
    return file;
}

std::unique_ptr<File> FileSystem::openWritable(const fs::path &path, bool append) const {
    const auto relative = normalizePath(path);
    requireWritable(relative);
    auto file = openFileForWriteImpl(relative, append);
    if (!file) {
        throw std::runtime_error("Filesystem returned a null file");
    }
    return file;
}

std::unique_ptr<File> FileSystem::openFileForWrite(const fs::path &path) const {
    return openWritable(path, false);
}
std::unique_ptr<File> FileSystem::openFileForAppend(const fs::path &path) const {
    return openWritable(path, true);
}
ByteArray FileSystem::readFile(const fs::path &path) const {
    return openFileForRead(path)->read();
}

void FileSystem::writeFile(const fs::path &path, std::span<const std::uint8_t> data) const {
    auto file = openFileForWrite(path);
    file->writeAll(data);
    file->flush();
}

void FileSystem::appendFile(const fs::path &path, std::span<const std::uint8_t> data) const {
    auto file = openFileForAppend(path);
    file->writeAll(data);
    file->flush();
}

bool FileSystem::isWritableImpl(const fs::path &) const {
    return false;
}

std::unique_ptr<File> FileSystem::openFileForWriteImpl(const fs::path &, bool) const {
    throw std::runtime_error("Filesystem is read-only");
}

void FileSystem::removeImpl(const fs::path &) const {
    throw std::runtime_error("Filesystem is read-only");
}
void FileSystem::createDirectoryImpl(const fs::path &) const {
    throw std::runtime_error("Filesystem is read-only");
}

VOLCANO_END
