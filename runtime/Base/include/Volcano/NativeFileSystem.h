#ifndef VOLCANO_NATIVEFILESYSTEM_H
#define VOLCANO_NATIVEFILESYSTEM_H

#include <Volcano/FileSystem.h>

#include <Volcano/Common.h>

VOLCANO_BEGIN

class NativeFileSystem final: public FileSystem {
public:
    explicit NativeFileSystem(const std::filesystem::path &root);

protected:
    bool isWritableImpl(const std::filesystem::path &) const override {
        return true;
    }
    
    DirEntry statImpl(const std::filesystem::path &path) const override;
    std::vector<DirEntry> listDirectoryImpl(const std::filesystem::path &path) const override;
    std::unique_ptr<File> openFileForReadImpl(const std::filesystem::path &path) const override;
    std::unique_ptr<File> openFileForWriteImpl(const std::filesystem::path &path, bool append) const override;
    void removeImpl(const std::filesystem::path &path) const override;
    void createDirectoryImpl(const std::filesystem::path &path) const override;

private:
    std::filesystem::path resolve(const std::filesystem::path &path) const;
    std::filesystem::path root_;
};

VOLCANO_END

#endif // VOLCANO_NATIVEFILESYSTEM_H
