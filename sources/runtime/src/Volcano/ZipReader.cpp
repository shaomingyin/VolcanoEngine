//
//
#include <memory>

#include <Volcano/ZipReader.hpp>

VOLCANO_BEGIN

ZipReader::ZipReader(void)
{
    mz_zip_zero_struct(&m_zipArchive);
}

ZipReader::~ZipReader(void)
{
    mz_zip_reader_end(&m_zipArchive);
}

bool ZipReader::init(const QString &fileName)
{
    return mz_zip_reader_init_file(&m_zipArchive, qPrintable(fileName), 0);
}

qint64 ZipReader::entryCount(void)
{
    return mz_zip_reader_get_num_files(&m_zipArchive);
}

int ZipReader::index(const QString &path)
{
    return mz_zip_reader_locate_file(&m_zipArchive, qPrintable(path), nullptr, 0);
}

bool ZipReader::stat(int index, Stat &out)
{
    return mz_zip_reader_file_stat(&m_zipArchive, index, &out);
}

bool ZipReader::stat(const QString path, Stat &out)
{
    return stat(index(path), out);
}

QByteArray ZipReader::extract(int index)
{
    Stat st;
    QByteArray buffer;

    if (!stat(index, st))
        return buffer;

    if (st.m_is_directory || st.m_uncomp_size == 0)
        return buffer;

    buffer.resize(st.m_uncomp_size);
    mz_zip_reader_extract_to_mem(&m_zipArchive, index, buffer.data(), st.m_uncomp_size, 0);

    return buffer;
}

QByteArray ZipReader::extract(const QString &fileName)
{
    return extract(index(fileName));
}

VOLCANO_END
