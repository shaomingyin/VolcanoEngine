//
//
#ifndef VOLCANO_IO_HPP
#define VOLCANO_IO_HPP

#include <Volcano/Common.hpp>

VOLCANO_BEGIN

class IO {
public:
    enum {
        FlagSeekable = 0x1,
        FlagReadable = 0x2,
        FlagWritable = 0x4
    };

    enum {
        ModeRead = 0x1,
        ModeWrite = 0x2,
        ModeAppend = 0x4,
        ModeCreate = 0x8,
        ModeTruncate = 0x10
    };

    enum class SeekMode {
        Set = 0,
        Cur,
        End
    };

public:
    IO(void);
    virtual ~IO(void);

public:
    virtual int flags(void) = 0;
    virtual int64_t size(void) = 0;
    virtual bool open(int mode);
    virtual void close(void);
    bool isOpen(void) const;
    int mode(void) const;
    virtual bool isEof(void) = 0;
    int64_t tell(void);
    int64_t seek(int64_t offset, SeekMode seekMode = SeekMode::Set);
    int64_t read(void *buf, int64_t len);
    int64_t write(const void *buf, int64_t len);

protected:
    virtual int64_t pos(void) = 0;
    virtual bool setPos(int64_t pos) = 0;
    virtual int64_t readData(void *buf, int64_t len) = 0;
    virtual int64_t writeData(const void *buf, int64_t len) = 0;

private:
    int m_mode;
};

VOLCANO_END

#endif // VOLCANO_IO_HPP
