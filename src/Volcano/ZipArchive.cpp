//
//
#include <QFileInfo>

#include <Volcano/ZipArchive.hpp>

VOLCANO_BEGIN

ZipArchive::ZipArchive(QObject *parent):
    QObject(parent)
{
}

ZipArchive::~ZipArchive(void)
{
    close();
}

bool ZipArchive::open(const QString &filename)
{
    QFileInfo fi(filename);
    if (!fi.isFile())
        return false;

    mz_zip_zero_struct(&m_archive);
    mz_bool mzret = mz_zip_reader_init_file(&m_archive, qPrintable(filename), 0);
    if (!mzret)
    {
        close();
        return false;
    }

    return true;
}

void ZipArchive::close(void)
{
    mz_zip_end(&m_archive);
}

int ZipArchive::fileCount(void)
{
    return mz_zip_reader_get_num_files(&m_archive);
}

int ZipArchive::fileLocation(const QString &filename)
{
    return mz_zip_reader_locate_file(&m_archive, qPrintable(filename) + 1, NULL, 0);
}

bool ZipArchive::fileStat(const QString &filename, mz_zip_archive_file_stat *st)
{
    return fileStat(fileLocation(filename), st);
}

bool ZipArchive::fileStat(int loc, mz_zip_archive_file_stat *st)
{
    Q_ASSERT(loc >= 0);
    Q_ASSERT(st != nullptr);

    return mz_zip_reader_file_stat(&m_archive, loc, st);
}

qint64 ZipArchive::fileSize(const QString &filename)
{
    return fileSize(fileLocation(filename));
}

qint64 ZipArchive::fileSize(int loc)
{
    mz_zip_archive_file_stat st;
    mz_zip_reader_file_stat(&m_archive, loc, &st);
    return st.m_uncomp_size;
}

bool ZipArchive::extractFile(const QString &filename, QByteArray &data)
{
    return extractFile(fileLocation(filename), data);
}

bool ZipArchive::extractFile(int loc, QByteArray &data)
{
    Q_ASSERT(loc >= 0);

    qint64 size = fileSize(loc);
    data.resize(size);
    mz_zip_reader_extract_to_mem(&m_archive, loc, data.data(), size, 0);

    return true;
}

VOLCANO_END
