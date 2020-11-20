//
//
#include <cctype>

#include <Volcano/ZipReader.hpp>

VOLCANO_BEGIN

ZipReader::ZipReader(void)
{
    mz_zip_zero_struct(&m_archive);
}

ZipReader::ZipReader(const char *filename) :
    m_filename(filename)
{
    mz_zip_zero_struct(&m_archive);
}

ZipReader::~ZipReader(void)
{
}

const std::string &ZipReader::filename(void) const
{
    return m_filename;
}

void ZipReader::setFilename(const std::string &r)
{
    VOLCANO_ASSERT(!isOpen());

    m_filename = r;
}

bool ZipReader::open(void)
{
    VOLCANO_ASSERT(isOpen());

    return mz_zip_reader_init_file(&m_archive, m_filename.c_str(), 0);
}

void ZipReader::close(void)
{
    VOLCANO_ASSERT(isOpen());

    mz_zip_reader_end(&m_archive);
}

bool ZipReader::isOpen(void) const
{
    return (m_archive.m_zip_mode == MZ_ZIP_MODE_READING);
}

int ZipReader::count(void)
{
    VOLCANO_ASSERT(isOpen());

    return mz_zip_reader_get_num_files(&m_archive);
}

int ZipReader::index(const std::string &path)
{
    VOLCANO_ASSERT(isOpen());

    if (path.empty())
        return -1;

    int head = 0;
    while (path[head] == '/')
        head += 1;

    int i = mz_zip_reader_locate_file(&m_archive, path.data() + head, nullptr, 0);
    if (i < 0)
        i = mz_zip_reader_locate_file(&m_archive, (path + '/').data() + head, nullptr, 0);

    return i;
}

bool ZipReader::stat(int index, Stat &r)
{
    VOLCANO_ASSERT(isOpen());
    VOLCANO_ASSERT(index >= 0);

    return mz_zip_reader_file_stat(&m_archive, index, &r);
}

bool ZipReader::stat(const std::string &path, Stat &r)
{
    VOLCANO_ASSERT(isOpen());

    int i = index(path);
    if (i < 0)
        return false;

    return stat(i, r);
}

bool ZipReader::extract(int i, ByteArray &ba)
{
    VOLCANO_ASSERT(isOpen());
    VOLCANO_ASSERT(i >= 0);

    Stat st;

    if (!stat(i, st) || st.m_is_directory)
        return false;

    ba.reserve(st.m_uncomp_size);

    return mz_zip_reader_extract_to_mem(&m_archive, i, ba.data(), st.m_uncomp_size, 0);
}

bool ZipReader::extract(const std::string &path, ByteArray &ba)
{
    VOLCANO_ASSERT(isOpen());

    int i = index(path);
    if (i < 0)
        return false;

    return extract(i, ba);
}

bool ZipReader::extract(int i, Buffer &buf)
{
    VOLCANO_ASSERT(isOpen());
    VOLCANO_ASSERT(i >= 0);

    return extract(i, buf.data);
}

bool ZipReader::extract(const std::string &path, Buffer &buf)
{
    VOLCANO_ASSERT(isOpen());

    int i = index(path);
    if (i < 0)
        return false;

    return extract(i, buf);
}

VOLCANO_END
