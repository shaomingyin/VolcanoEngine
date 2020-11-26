//
//
#ifndef VOLCANO_ZIPREADER_HPP
#define VOLCANO_ZIPREADER_HPP

#include <string>

#include <miniz_zip.h>

#include <Volcano/Common.hpp>
#include <Volcano/Buffer.hpp>

VOLCANO_BEGIN

class ZipReader: public Noncopyable {
public:
    typedef mz_zip_archive_file_stat Stat;

public:
    ZipReader(void);
    ZipReader(const char *filename);
    virtual ~ZipReader(void);

public:
    const std::string &filename(void) const;
    void setFilename(const std::string &r);
    bool open(void);
    void close(void);
    bool isOpen(void) const;
    int count(void);
    int index(const std::string &path);
    bool stat(int i, Stat &r);
    bool stat(const std::string &path, Stat &r);
    bool extract(int i, ByteArray &ba);
    bool extract(const std::string &path, ByteArray &ba);
    bool extract(int i, Buffer &buf);
    bool extract(const std::string &path, Buffer &buf);

private:
    std::string m_filename;
    mz_zip_archive m_archive;
};

VOLCANO_END

#endif // VOLCANO_ZIPARCHIVE_HPP
