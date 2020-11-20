//
//
#include <filesystem>

#include <Volcano/VM/ZipFileSystem.hpp>
#include <Volcano/VM/RootFileSystem.hpp>

VOLCANO_VM_BEGIN

RootFileSystem::RootFileSystem(void)
{
}

RootFileSystem::~RootFileSystem(void)
{
}

RootFileSystem::Type RootFileSystem::type(const std::string &path)
{
    Type type = FileSystem::type(path);
    if (type != TYPE_INVALID)
        return type;

    for (auto it(m_mountList.begin()); it != m_mountList.end(); ++it) {
        if (it->path.size() < path.size())
            continue;
        if (strncmp(it->path.data(), path.data(), it->path.size()))
            continue;
        type = it->fs->type(path.data() + it->path.size());
        if (type != TYPE_INVALID)
            return type;
    }

    return TYPE_INVALID;
}

bool RootFileSystem::enumDir(const std::string &path, StringList &result)
{
    result.clear();
    FileSystem::enumDir(path, result);

    for (auto it(m_mountList.begin()); it != m_mountList.end(); ++it) {
        if (it->path.size() < path.size())
            continue;
        if (strncmp(it->path.data(), path.data(), it->path.size()))
            continue;
        StringList tmp;
        if (it->fs->enumDir(path.data() + it->path.size(), tmp))
            result.merge(tmp);
    }

    result.sort();

    return true;
}

IO *RootFileSystem::openFile(const std::string &path, int ioFlags)
{
    IO *p = FileSystem::openFile(path, ioFlags);
    if (p != nullptr)
        return p;

    if (ioFlags & (IO::FLAG_WRITE | IO::FLAG_CREATE | IO::FLAG_TRUNCATE))
        return nullptr;

    for (auto it(m_mountList.begin()); it != m_mountList.end(); ++it) {
        if (it->path.size() < path.size())
            continue;
        if (strncmp(it->path.data(), path.data(), it->path.size()))
            continue;
        IO *p = it->fs->openFile(path.data() + it->path.size(), ioFlags);
        if (p != nullptr)
            return p;
    }

    return nullptr;
}

bool RootFileSystem::mount(const std::string &nativePath, const std::string &path)
{
    if (!checkPath(path))
        return false;

    if (findMount(nativePath, path) != m_mountList.end())
        return false;

    FileSystemPtr p;

    if (std::filesystem::is_directory(nativePath))
        p.reset(new FileSystem());
    else if (std::filesystem::is_regular_file(nativePath))
        p.reset(new ZipFileSystem());

    if (!p || !p->init(nativePath))
        return false;

    std::string tmp(path);
    if (tmp.back() != '/')
        tmp += '/';

    m_mountList.emplace_front(p, tmp);

    return true;
}

bool RootFileSystem::unmount(const std::string &nativePath, const std::string &path)
{
    auto it(findMount(nativePath, path));
    if (it == m_mountList.end())
        return false;

    m_mountList.erase(it);

    return true;
}

const MountInfoList &RootFileSystem::mountList(void) const
{
    return m_mountList;
}

bool RootFileSystem::isWritable(const std::string &path)
{
    return (FileSystem::type(path) != TYPE_INVALID);
}

MountInfoList::iterator RootFileSystem::findMount(const std::string &nativePath, const std::string &path)
{
    std::string tmp(path);
    if (tmp.back() != '/')
        tmp += '/';

    for (auto it(m_mountList.begin()); it != m_mountList.end(); ++it) {
        if (it->fs->nativePath() == nativePath && it->path == tmp)
            return it;
    }

    return m_mountList.end();
}

VOLCANO_VM_END
