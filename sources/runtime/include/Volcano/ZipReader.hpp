//
//
#ifndef VOLCANO_ZIPREADER_HPP
#define VOLCANO_ZIPREADER_HPP

#include <miniz.h>

#include <QString>
#include <QByteArray>

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class ZipReader {
public:
    using Stat = mz_zip_archive_file_stat;

public:
    ZipReader(void);
    virtual ~ZipReader(void);

public:
    bool init(const QString &fileName);
    qint64 entryCount(void);
    int index(const QString &path);
    bool stat(int index, Stat &out);
    bool stat(const QString path, Stat &out);
    QByteArray extract(int index);
    QByteArray extract(const QString &fileName);

private:
    mz_zip_archive m_zipArchive;
};

VOLCANO_END

#endif // VOLCANO_ZIPREADER_HPP
