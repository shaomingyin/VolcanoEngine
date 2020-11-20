//
//
#ifndef VOLCANO_VM_ROOTFILESYSTEM_HPP
#define VOLCANO_VM_ROOTFILESYSTEM_HPP

#include <list>
#include <memory>

#include <Volcano/VM/Common.hpp>
#include <Volcano/VM/FileSystem.hpp>

VOLCANO_VM_BEGIN

typedef std::shared_ptr<FileSystem> FileSystemPtr;

struct MountInfo {
    MountInfo(FileSystemPtr f, const std::string &p): fs(f), path(p) { }

    FileSystemPtr fs;
    std::string path;
};

typedef std::list<MountInfo> MountInfoList;

class RootFileSystem: public FileSystem {
public:
    RootFileSystem(void);
    ~RootFileSystem(void) override;

public:
    Type type(const std::string &path) override;
    bool enumDir(const std::string &path, StringList &result) override;
    IO *openFile(const std::string &path, int io_flags) override;
    bool mount(const std::string &native_path, const std::string &path);
    bool unmount(const std::string &native_path, const std::string &path);
    const MountInfoList &mountList(void) const;
    bool isWritable(const std::string &path);

protected:
    MountInfoList::iterator findMount(const std::string &nativePath, const std::string &path);

private:
    MountInfoList m_mountList;
};

VOLCANO_VM_END

#endif // VOLCANO_VM_ROOTFILESYSTEM_HPP
