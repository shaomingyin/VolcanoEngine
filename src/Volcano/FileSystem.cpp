//
//
#include <QChar>
#include <QDir>
#include <QFile>
#include <QFileInfo>

#include <Volcano/FileSystem.hpp>

VOLCANO_BEGIN

FileSystem::FileSystem(QObject *parent):
    QObject(parent)
{
}

FileSystem::~FileSystem(void)
{
}

bool FileSystem::init(const QString &nativePath)
{
    QFileInfo fi(nativePath);
    if (!fi.isDir())
        return false;

    m_nativePath = nativePath;

    if (!m_nativePath.endsWith('/'))
        m_nativePath += '/';

    return true;
}

FileSystem::Type FileSystem::type(const QString &path)
{
    if (!checkPath(path))
        return TypeUnknown;

    QFileInfo fi(m_nativePath + path);
    if (fi.isDir())
        return TypeDir;
    if (fi.isFile())
        return TypeFile;

    return TypeUnknown;
}

bool FileSystem::enumDir(const QString &dirname, QStringList &entryList)
{
    if (!checkPath(dirname))
        return false;

    QDir dir(m_nativePath + dirname);
    entryList += dir.entryList();

    return true;
}

FilePtr FileSystem::openFile(const QString &filename, QIODevice::OpenMode mode)
{
    if (!checkPath(filename))
        return FilePtr();

    FilePtr file(new QFile(m_nativePath + filename));
    if (!file || !file->open(mode))
        return FilePtr();

    return file;
}

bool FileSystem::checkNameChar(QChar c)
{
    if (c.isLetterOrNumber())
        return true;

    if (c == '.' || c == '_')
        return true;

    return false;
}

bool FileSystem::checkName(const QString &name)
{
    int size = name.size();

    for (int i = 0; i < size; ++i)
    {
        if (!checkNameChar(name.at(i)))
            return false;
    }

    return true;
}

bool FileSystem::checkPath(const QString &path)
{
    if (path.isNull() || path.isEmpty())
        return false;

    if (path.at(0) != '/')
        return false;

    int size = path.size();
    bool slashed = true;

    for (int i = 1; i < size; ++i)
    {
        QChar c = path.at(i);
        if (c == '/')
        {
            if (slashed)
                return false;
            slashed = true;
        }
        else
        {
            if (!checkNameChar(c))
                return false;
        }
    }

    return true;
}

VOLCANO_END
