#ifndef VOLCANO_DIRENTRY_H
#define VOLCANO_DIRENTRY_H

#include <cstddef>
#include <filesystem>
#include <stdexcept>
#include <utility>

#include <Volcano/Common.h>

VOLCANO_BEGIN

enum class FileType {
    Missing,
    Regular,
    Directory,
    Other
};

// Metadata is a snapshot; query the filesystem again to refresh it.
class DirEntry {
public:
    explicit DirEntry(std::filesystem::path path, FileType type = FileType::Missing, std::size_t size = 0)
        : path_(std::move(path)), type_(type), size_(size) {
    }

    const std::filesystem::path &path() const {
        return path_;
    }
    
    FileType type() const {
        return type_;
    }

    bool exists() const {
        return type_ != FileType::Missing;
    }
    bool isFile() const {
        return type_ == FileType::Regular;
    }
    bool isDirectory() const {
        return type_ == FileType::Directory;
    }

    std::size_t size() const {
        if (!isFile()) {
            throw std::runtime_error("Entry is not a regular file: " + path_.generic_string());
        }
        return size_;
    }

private:
    std::filesystem::path path_;
    FileType type_;
    std::size_t size_;
};

VOLCANO_END

#endif // VOLCANO_DIRENTRY_H
