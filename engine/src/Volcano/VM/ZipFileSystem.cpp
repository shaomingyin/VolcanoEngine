//
//
#include <algorithm>
#include <filesystem>
#include <string_view>

#include <Volcano/VM/ZipFileSystem.hpp>

VOLCANO_VM_BEGIN

// ZipFile

class ZipFile: public IO {
public:
    ZipFile(ZipReaderPtr &reader, int index);
    ~ZipFile(void) override;

public:
    bool open(int flags) override;
    void close(void) override;
    int64_t size(void) override;
    int64_t read(void *p, int64_t size) override;
    int64_t write(const void *p, int64_t size) override;

protected:
    int64_t pos(void) override;
    int64_t setPos(int64_t v) override;

private:
    ZipReaderPtr m_reader;
    int m_index;
    ByteArray m_data;
    int64_t m_pos;
};

ZipFile::ZipFile(ZipReaderPtr &reader, int index):
    m_reader(reader),
    m_index(index),
    m_pos(0)
{
    VOLCANO_ASSERT(m_reader);
    VOLCANO_ASSERT(m_index >= 0);
}

ZipFile::~ZipFile(void)
{
    if (isOpen())
        close();
}

bool ZipFile::open(int flags)
{
    VOLCANO_ASSERT(m_reader);
    VOLCANO_ASSERT(m_index >= 0);

    if (flags & (FLAG_WRITE | FLAG_CREATE | FLAG_TRUNCATE))
        return false;

    if (!IO::open(flags))
        return false;

    ZipReader::Stat st;
    if (!m_reader->stat(m_index, st)) {
        IO::close();
        return false;
    }

    if (!m_reader->extract(st.m_file_index, m_data)) {
        IO::close();
        return false;
    }

    setSeekable(true);
    m_pos = 0;

    return true;
}

void ZipFile::close(void)
{
    m_data.clear();
    m_data.reserve(0);

    IO::close();
}

int64_t ZipFile::size(void)
{
    VOLCANO_ASSERT(isOpen());

    return m_data.size();
}

int64_t ZipFile::read(void *p, int64_t size)
{
    VOLCANO_ASSERT(isOpen());

    int64_t len = std::min(int64_t(m_data.size()) - m_pos, size);
    if (len > 0) {
        if (p != nullptr)
            memcpy(p, m_data.data() + m_pos, len);
        m_pos += len;
    }

    return len;
}

int64_t ZipFile::write(const void *p, int64_t size)
{
    VOLCANO_ASSERT(isOpen());

    int64_t len = std::min(int64_t(m_data.size()) - m_pos, size);
    if (len > 0) {
        if (p != nullptr)
            memcpy(m_data.data() + m_pos, p, len);
        m_pos += len;
    }

    return len;
}

int64_t ZipFile::pos(void)
{
    return m_pos;
}

int64_t ZipFile::setPos(int64_t v)
{
    m_pos = std::clamp(v, int64_t(0), int64_t(m_data.size()));
    return m_pos;
}

// ZipFileSystem

ZipFileSystem::ZipFileSystem(void):
    m_tree(0)
{
}

ZipFileSystem::~ZipFileSystem(void)
{
}

bool ZipFileSystem::isWritable(void)
{
    return false;
}

bool ZipFileSystem::init(const std::string &nativePath)
{
    VOLCANO_ASSERT(!m_reader);

    if (!std::filesystem::is_regular_file(nativePath))
        return false;

    auto reader = std::make_shared<ZipReader>(nativePath.c_str());
    if (!reader || !reader->open())
        return false;

    m_tree.clear();
    m_tree.reserve(0);

    int n = reader->count();
    if (n < 1)
        return true;

    m_tree.reserve(n);

    for (int i = 0; i < n; ++i) {
        ZipReader::Stat st;
        if (reader->stat(i, st)) {
            if (!populateNode(st))
                return false;
        }
    }

    m_reader = reader;
    setNativePath(nativePath);

    return true;
}

FileSystem::Type ZipFileSystem::type(const std::string &path)
{
    VOLCANO_ASSERT(m_reader);

    if (!checkPath(path))
        return TYPE_INVALID;

    ZipReader::Stat st;
    if (!m_reader->stat(path, st))
        return TYPE_INVALID;

    if (st.m_is_directory)
        return TYPE_DIR;

    return TYPE_FILE;
}

bool ZipFileSystem::enumDir(const std::string &path, StringList &result)
{
    VOLCANO_ASSERT(m_reader);

    if (!checkPath(path))
        return false;

    ZipReader::Stat st;
    if (!m_reader->stat(path, st))
        return false;
    if (!st.m_is_directory)
        return false;

    result.clear();

    for (int i = m_tree[st.m_file_index].firstChild; i >= 0; i = m_tree[i].nextSibling)
        result.push_back(m_tree[i].name);

    return true;
}

bool ZipFileSystem::makeDir(const std::string &path)
{
    VOLCANO_ASSERT(m_reader);
    VOLCANO_UNUSED(path);

    return false;
}

IO *ZipFileSystem::openFile(const std::string &path, int ioFlags)
{
    VOLCANO_ASSERT(m_reader);

    if (!checkPath(path))
        return nullptr;

    if (path.back() == '/')
        return nullptr;

    if (ioFlags & (IO::FLAG_WRITE | IO::FLAG_CREATE | IO::FLAG_TRUNCATE))
        return nullptr;

    int i = m_reader->index(path);
    if (i < 0)
        return nullptr;

    ZipFile *fp = new ZipFile(m_reader, i);
    if (fp == nullptr)
        return nullptr;

    if (!fp->open(ioFlags)) {
        delete fp;
        return nullptr;
    }

    return fp;
}

bool ZipFileSystem::remove(const std::string &path)
{
    VOLCANO_ASSERT(m_reader);
    VOLCANO_UNUSED(path);

    return false;
}

bool ZipFileSystem::populateNode(const ZipReader::Stat &st)
{
    int parent = 0;
    std::filesystem::path path(st.m_filename);

    for (auto it(path.begin()); it != path.end(); ++it) {
        if (it->filename().empty())
            break;
        int i = findChild(it->filename(), parent);
        if (i < 0) {
            ZipReader::Stat st;
            if (!m_reader->stat(it->string(), st))
                return false;
            m_tree[st.m_file_index].name = it->filename();
            m_tree[st.m_file_index].nextSibling = m_tree[parent].firstChild;
            m_tree[parent].firstChild = st.m_file_index;
        }
        parent = i;
    }

    return true;
}

int ZipFileSystem::findChild(const std::string &name, int parent)
{
    for (int i = m_tree[parent].firstChild; i >= 0; i = m_tree[i].nextSibling) {
        if (name == m_tree[i].name)
            return i;
        i = m_tree[i].nextSibling;
    }

    return -1;
}

VOLCANO_VM_END
