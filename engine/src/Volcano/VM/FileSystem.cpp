//
//
#include <cctype>

#include <Volcano/File.hpp>
#include <Volcano/VM/FileSystem.hpp>

VOLCANO_VM_BEGIN

FileSystem::FileSystem(void)
{
}

FileSystem::~FileSystem(void)
{
}

bool FileSystem::isWritable(void)
{
    VOLCANO_ASSERT(!m_nativePath.empty());

    return true;
}

bool FileSystem::init(const std::string &nativePath)
{
    VOLCANO_ASSERT(m_nativePath.empty());

    if (!std::filesystem::is_directory(nativePath))
        return false;

    if (!isWritablePath(nativePath))
        return false;

    m_nativePath = std::filesystem::canonical(nativePath);

    return true;
}

void FileSystem::shutdown(void)
{
    VOLCANO_ASSERT(!m_nativePath.empty());

    m_nativePath.clear();
}

FileSystem::Type FileSystem::type(const std::string &path)
{
    VOLCANO_ASSERT(!m_nativePath.empty());

    if (!checkPath(path))
        return TYPE_INVALID;

    if (std::filesystem::is_directory(m_nativePath/path))
        return TYPE_DIR;

    if (std::filesystem::is_regular_file(m_nativePath/path))
        return TYPE_FILE;

    return TYPE_INVALID;
}

bool FileSystem::enumDir(const std::string &path, StringList &result)
{
    VOLCANO_ASSERT(!m_nativePath.empty());

    if (!checkPath(path))
        return false;

    if (!exists(path))
        return false;

    result.clear();

    for (auto &dent: std::filesystem::directory_iterator(m_nativePath/path)) {
        result.push_back(dent.path().stem());
    }

    return true;
}

bool FileSystem::makeDir(const std::string &path)
{
    VOLCANO_ASSERT(!m_nativePath.empty());

    if (!checkPath(path))
        return false;

    return std::filesystem::create_directories(m_nativePath/path);
}

IO *FileSystem::openFile(const std::string &path, int ioFlags)
{
    VOLCANO_ASSERT(!m_nativePath.empty());

    if (!checkPath(path))
        return nullptr;

    File *fp = new File((m_nativePath/path).string());
    if (fp == nullptr)
        return nullptr;

    if (!fp->open(ioFlags)) {
        delete fp;
        return nullptr;
    }

    return fp;
}

bool FileSystem::remove(const std::string &path)
{
    VOLCANO_ASSERT(!m_nativePath.empty());

    if (!checkPath(path))
        return false;

    return std::filesystem::remove(m_nativePath/path);
}

bool FileSystem::checkName(const std::string &name)
{
    for (size_t i = 0; i < name.size(); ++i) {
        if (!isNameChar(name[i]))
            return false;
    }

    return true;
}

bool FileSystem::checkPath(const std::string &path)
{
    if (path.empty())
        return false;

    if (path[0] != '/')
        return false;

    bool slashed = true;

    for (size_t i = 1; i < path.size(); ++i) {
        char c = path[i];
        if (c == '/') {
            if (slashed)
                return false;
            slashed = true;
        } else if (!isNameChar(c))
            return false;
        else
            slashed = false;
    }

    return true;
}

void FileSystem::setNativePath(const std::string &r)
{
    m_nativePath = std::filesystem::canonical(r);
}

bool FileSystem::isNameChar(char c)
{
    if (std::isalnum(c))
        return true;

    if (c == '_' || c == '.')
        return true;

    return false;
}

VOLCANO_VM_END
