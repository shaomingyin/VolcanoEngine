//
//
#include <Volcano/File.hpp>

VOLCANO_BEGIN

// FileBase

FileBase::FileBase(void):
    m_handle(nullptr)
{
}

FileBase::FileBase(std::string_view path):
    m_path(path),
    m_handle(nullptr)
{
}

FileBase::~FileBase(void)
{
    if (m_handle != nullptr)
        PHYSFS_close(m_handle);
}

int64_t FileBase::size(void)
{
    VOLCANO_ASSERT(flags() & FlagSeekable);

    Stat out;

    if (!PHYSFS_stat(m_path.c_str(), &out))
        return -1;

    return out.filesize;
}

int64_t FileBase::size(std::string_view path)
{
    Stat out;

    if (!PHYSFS_stat(path.data(), &out))
        return -1;

    return out.filesize;
}

void FileBase::close(void)
{
    VOLCANO_ASSERT(m_handle != nullptr);

    PHYSFS_close(m_handle);
    m_handle = nullptr;

    IO::close();
}

bool FileBase::isEof(void)
{
    VOLCANO_ASSERT(m_handle != nullptr);

    return PHYSFS_eof(m_handle);
}

const std::string &FileBase::path(void) const
{
    return m_path;
}

void FileBase::setPath(std::string_view path)
{
    VOLCANO_ASSERT(m_handle == nullptr);

    m_path = path;
}

bool FileBase::stat(Stat &out)
{
    return PHYSFS_stat(m_path.c_str(), &out);
}

bool FileBase::stat(std::string_view path, Stat &out)
{
    return PHYSFS_stat(path.data(), &out);
}

bool FileBase::isFile(std::string_view path)
{
    return PHYSFS_exists(path.data());
}

int64_t FileBase::pos(void)
{
    VOLCANO_ASSERT(m_handle != nullptr);

    return PHYSFS_tell(m_handle);
}

bool FileBase::setPos(int64_t pos)
{
    VOLCANO_ASSERT(m_handle != nullptr);
    VOLCANO_ASSERT(flags() & FlagSeekable);

    return PHYSFS_seek(m_handle, pos);
}

int64_t FileBase::readData(void *buf, int64_t len)
{
    VOLCANO_ASSERT(m_handle != nullptr);
    VOLCANO_ASSERT(flags() & FlagReadable);

    return PHYSFS_readBytes(m_handle, buf, len);
}

int64_t FileBase::writeData(const void *buf, int64_t len)
{
    VOLCANO_ASSERT(m_handle != nullptr);
    VOLCANO_ASSERT(flags() & FlagWritable);

    return PHYSFS_writeBytes(m_handle, buf, len);
}

// FileReader

FileReader::FileReader(void)
{
}

FileReader::FileReader(std::string_view &path):
    FileBase(path)
{
}

FileReader::~FileReader(void)
{
}

int FileReader::flags(void)
{
    return FlagReadable | FlagSeekable;
}

bool FileReader::open(int mode)
{
    VOLCANO_ASSERT(m_handle == nullptr);

    if (mode & (ModeWrite | ModeAppend | ModeCreate | ModeTruncate))
        return false;

    if (!FileBase::open(mode))
        return false;

    m_handle = PHYSFS_openRead(m_path.c_str());
    if (m_handle == nullptr) {
        FileBase::close();
        return false;
    }

    return true;
}

// FileWriter

FileWriter::FileWriter(void)
{
}

FileWriter::FileWriter(std::string_view &path):
    FileBase(path)
{
}

FileWriter::~FileWriter(void)
{
}

int FileWriter::flags(void)
{
    return FlagWritable | FlagSeekable;
}

bool FileWriter::open(int mode)
{
    VOLCANO_ASSERT(m_handle == nullptr);

    if (mode & ModeRead)
        return false;

    if (!FileBase::open(mode))
        return false;

    if (mode & ModeWrite)
        m_handle = PHYSFS_openWrite(m_path.c_str());
    else if (mode & ModeAppend)
        m_handle = PHYSFS_openAppend(m_path.c_str());

    if (m_handle == nullptr) {
        FileBase::close();
        return false;
    }

    return true;
}

VOLCANO_END
