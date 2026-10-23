#ifndef VOLCANO_VIRTUALFILESYSTEM_H
#define VOLCANO_VIRTUALFILESYSTEM_H

#include <mutex>

#include <Volcano/FileSystem.h>

#include <Volcano/Common.h>

VOLCANO_BEGIN

class VirtualFileSystem final: public FileSystem {
public:
    // Deeper mount points take precedence; append puts a layer last at the same point.
    void mount(const std::filesystem::path &path, std::shared_ptr<FileSystem> filesystem, bool append = false);
    void mount(const std::filesystem::path &path, const std::filesystem::path &osPath, bool append = false);
    // Remove all layers at this mount point.
    void unmount(const std::filesystem::path &path);

protected:
    bool isWritableImpl(const std::filesystem::path &path) const override;
    DirEntry statImpl(const std::filesystem::path &path) const override;
    std::vector<DirEntry> listDirectoryImpl(const std::filesystem::path &path) const override;
    std::unique_ptr<File> openFileForReadImpl(const std::filesystem::path &path) const override;
    std::unique_ptr<File> openFileForWriteImpl(const std::filesystem::path &path, bool append) const override;
    void removeImpl(const std::filesystem::path &path) const override;
    void createDirectoryImpl(const std::filesystem::path &path) const override;

private:
    struct Mount {
        std::filesystem::path point;
        std::shared_ptr<FileSystem> filesystem;
    };

    struct State {
        std::vector<Mount> mounts;
    };

    struct Lookup {
        DirEntry entry;
        std::shared_ptr<FileSystem> filesystem;
        std::filesystem::path relative;
    };

    static const Mount *selectMount(const State &view, const std::filesystem::path &path);
    State snapshot() const;
    bool reaches(const VirtualFileSystem *filesystem) const;
    Lookup lookup(const State &view, const std::filesystem::path &path) const;
    Lookup visible(const State &view, const std::filesystem::path &path) const;
    Lookup writable(const State &view, const std::filesystem::path &path) const;
    std::vector<DirEntry> children(const State &view, const std::filesystem::path &path) const;

    static std::mutex topologyMutex_;
    mutable std::mutex stateMutex_;
    State state_;
};

VOLCANO_END

#endif // VOLCANO_VIRTUALFILESYSTEM_H
