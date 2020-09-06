//
//
#ifndef VOLCANO_ZIPARCHIVE_HPP
#define VOLCANO_ZIPARCHIVE_HPP

#include <miniz.h>

#include <QString>
#include <QObject>
#include <QSharedPointer>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class VOLCANO_API ZipArchive: public QObject
{
    Q_OBJECT

public:
    ZipArchive(QObject *parent = nullptr);
    virtual ~ZipArchive(void);

public:
    bool open(const QString &filename);
    void close(void);
    int fileCount(void);
    int fileLocation(const QString &filename);
    bool fileStat(const QString &filename, mz_zip_archive_file_stat *st);
    bool fileStat(int loc, mz_zip_archive_file_stat *st);
    qint64 fileSize(const QString &filename);
    qint64 fileSize(int loc);
    bool extractFile(const QString &filename, QByteArray &data);
    bool extractFile(int loc, QByteArray &data);
    mz_zip_archive *handle(void);

private:
    mz_zip_archive m_archive;
};

typedef QSharedPointer<ZipArchive> ZipArchivePtr;

VOLCANO_INLINE mz_zip_archive *ZipArchive::handle(void)
{
    return &m_archive;
}

VOLCANO_END

#endif // VOLCANO_ZIPARCHIVE_HPP
