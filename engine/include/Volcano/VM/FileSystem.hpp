//
//
#ifndef VOLCANO_VM_FILESYSTEM_HPP
#define VOLCANO_VM_FILESYSTEM_HPP

#include <string>
#include <filesystem>

#include <Volcano/IO.hpp>
#include <Volcano/VM/Common.hpp>

VOLCANO_VM_BEGIN

class FileSystem: public Noncopyable {
public:
    enum Type {
        TYPE_INVALID = 0,
        TYPE_DIR,
        TYPE_FILE
    };

public:
    FileSystem(void);
    virtual ~FileSystem(void);

public:
    virtual bool isWritable(void);
    virtual bool init(const std::string &native_path);
    virtual void shutdown(void);
    const std::filesystem::path &nativePath(void) const;
    virtual Type type(const std::string &path);
    bool exists(const std::string &path);
    virtual bool enumDir(const std::string &path, StringList &result);
    virtual bool makeDir(const std::string &path);
    virtual IO *openFile(const std::string &path, int ioFlags);
    virtual bool remove(const std::string &path);
    static bool checkName(const std::string &name);
    static bool checkPath(const std::string &path);

protected:
    void setNativePath(const std::string &r);
    static bool isNameChar(char c);

private:
    std::filesystem::path m_nativePath;
};

VOLCANO_INLINE const std::filesystem::path &FileSystem::nativePath(void) const
{
    return m_nativePath;
}

VOLCANO_INLINE bool FileSystem::exists(const std::string &path)
{
    return (type(path) != TYPE_INVALID);
}

VOLCANO_VM_END

#endif // VOLCANO_VM_FILESYSTEM_HPP
