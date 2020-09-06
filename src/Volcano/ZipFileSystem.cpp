//
//
#include <QFileInfo>
#include <QBuffer>

#include <Volcano/ZipFileSystem.hpp>

VOLCANO_BEGIN

ZipFileSystem::ZipFileSystem(QObject *parent):
    FileSystem(parent)
{
}

ZipFileSystem::~ZipFileSystem(void)
{
}

bool ZipFileSystem::init(const QString &nativePath)
{
    Q_ASSERT(!m_archive);

    QFileInfo fi(nativePath);
    if (!fi.isFile())
        return false;

    ZipArchivePtr archive(new ZipArchive());
    if (!archive || !archive->open(nativePath))
        return false;

    int n = archive->fileCount();
    for (int i = 0; i < n; ++i)
    {
        mz_zip_archive_file_stat st;
        if (archive->fileStat(i, &st))
        {
            QString n("/");
            n += st.m_filename;
            m_statMap[n] = st;
        }
    }

    m_archive = archive;
    m_nativePath = nativePath;

    return true;
}

FileSystem::Type ZipFileSystem::type(const QString &path)
{
    if (!checkPath(path))
        return TypeUnknown;

    QString tmp(path);
    if (tmp.size() > 1 && !tmp.endsWith('/'))
        tmp += '/';

    if (m_statMap.contains(tmp))
        return TypeDir;

    if (!m_statMap.contains(path))
        return TypeUnknown;

    const mz_zip_archive_file_stat *st = &m_statMap[path];
    if (st->m_is_directory)
        return TypeDir;

    return TypeFile;
}

bool ZipFileSystem::enumDir(const QString &dirname, QStringList &entryList)
{
    if (!checkPath(dirname))
        return false;

    QString tmp(dirname);
    if (tmp.size() > 1 && !tmp.endsWith('/'))
        tmp += '/';

    if (!m_statMap.contains(tmp))
        return false;

    for (auto it(m_statMap.begin()); it != m_statMap.end(); ++it)
    {
        if (!it.key().startsWith(tmp))
            continue;
        QString path = it.key();
        path.remove(0, tmp.length());
        entryList.append(path.section('/', 1, 1));
    }

    return true;
}

FilePtr ZipFileSystem::openFile(const QString &filename, QIODevice::OpenMode mode)
{
    if (!checkPath(filename))
        return FilePtr();

    if (mode & (QIODevice::WriteOnly | QIODevice::Append | QIODevice::Truncate | QIODevice::NewOnly))
        return FilePtr();

    QBuffer *buf = new QBuffer();
    if (buf == nullptr)
        return FilePtr();

    if (!m_archive->extractFile(filename, buf->buffer()))
    {
        delete buf;
        return FilePtr();
    }

    return FilePtr(buf);
}

VOLCANO_END
