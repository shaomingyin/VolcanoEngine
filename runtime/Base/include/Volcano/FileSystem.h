#ifndef VOLCANO_FILESYSTEM_H
#define VOLCANO_FILESYSTEM_H

#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <system_error>
#include <vector>

#include <Volcano/DirEntry.h>
#include <Volcano/File.h>

#include <Volcano/Common.h>

VOLCANO_BEGIN

// Virtual paths start with '/'; '..' may ascend within this instance's root. There is no CWD.
class FileSystem {
public:
    using DirectoryCallback = std::function<bool(const DirEntry &)>;

    FileSystem() = default;
    virtual ~FileSystem() = default;
    FileSystem(const FileSystem &) = delete;
    FileSystem &operator=(const FileSystem &) = delete;

    static std::string toNativeString(const std::filesystem::path &path);
    bool isWritable(const std::filesystem::path &path) const;

    DirEntry stat(const std::filesystem::path &path) const;
    bool exists(const std::filesystem::path &path) const;
    bool isFile(const std::filesystem::path &path) const;
    bool isDirectory(const std::filesystem::path &path) const;
    std::size_t fileSize(const std::filesystem::path &path) const;
    void remove(const std::filesystem::path &path) const;
    void createDirectory(const std::filesystem::path &path) const;
    // Entries contain absolute virtual paths; false stops the entire traversal.
    void listDirectory(const std::filesystem::path &path, const DirectoryCallback &callback, bool recursive = false) const;
    std::vector<DirEntry> listDirectory(const std::filesystem::path &path, bool recursive = false) const;
    std::unique_ptr<File> openFileForRead(const std::filesystem::path &path) const;
    std::unique_ptr<File> openFileForWrite(const std::filesystem::path &path) const;
    std::unique_ptr<File> openFileForAppend(const std::filesystem::path &path) const;
    ByteArray readFile(const std::filesystem::path &path) const;
    void writeFile(const std::filesystem::path &path, std::span<const std::uint8_t> data) const;
    void appendFile(const std::filesystem::path &path, std::span<const std::uint8_t> data) const;

protected:
    static std::filesystem::path normalizePath(const std::filesystem::path &path);
    static bool isMissing(const std::error_code &error);
    static std::filesystem::path virtualPath(const std::filesystem::path &path);
    // Component-wise operations on normalized paths; relativeTo requires isPrefix(prefix, path).
    static bool isPrefix(const std::filesystem::path &prefix, const std::filesystem::path &path);
    static std::filesystem::path relativeTo(const std::filesystem::path &path, const std::filesystem::path &prefix);

    // Hooks receive normalized relative paths; empty denotes root. List returns child names.
    virtual bool isWritableImpl(const std::filesystem::path &path) const;
    virtual DirEntry statImpl(const std::filesystem::path &path) const = 0;
    virtual std::vector<DirEntry> listDirectoryImpl(const std::filesystem::path &path) const = 0;
    virtual std::unique_ptr<File> openFileForReadImpl(const std::filesystem::path &path) const = 0;
    virtual std::unique_ptr<File> openFileForWriteImpl(const std::filesystem::path &path, bool append) const;
    virtual void removeImpl(const std::filesystem::path &path) const;
    virtual void createDirectoryImpl(const std::filesystem::path &path) const;

private:
    void requireWritable(const std::filesystem::path &path) const;
    std::vector<DirEntry> children(const std::filesystem::path &path) const;
    bool enumerate(const std::filesystem::path &path, const DirectoryCallback &callback, bool recursive) const;
    std::unique_ptr<File> openWritable(const std::filesystem::path &path, bool append) const;
};

VOLCANO_END

#endif // VOLCANO_FILESYSTEM_H
