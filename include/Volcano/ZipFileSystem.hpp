//
//
#ifndef VOLCANO_ZIPFILESYSTEM_HPP
#define VOLCANO_ZIPFILESYSTEM_HPP

#include <miniz.h>

#include <QMap>

#include <Volcano/Common.hpp>
#include <Volcano/FileSystem.hpp>
#include <Volcano/ZipArchive.hpp>

VOLCANO_BEGIN

class VOLCANO_API ZipFileSystem: public FileSystem
{
    Q_OBJECT

public:
    ZipFileSystem(QObject *parent = nullptr);
    ~ZipFileSystem(void);

public:
    bool init(const QString &nativePath) override;
    Type type(const QString &path) override;
    bool enumDir(const QString &dirname, QStringList &entryList) override;
    FilePtr openFile(const QString &filename, QIODevice::OpenMode mode) override;

private:
    typedef QMap<QString, mz_zip_archive_file_stat> StatMap;

    ZipArchivePtr m_archive;
    StatMap m_statMap;
};

VOLCANO_END

#endif // VOLCANO_ZIPFILESYSTEM_HPP
