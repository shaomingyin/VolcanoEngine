//
//
#ifndef VOLCANO_MODELCOMPILER_IOSTREAM_H
#define VOLCANO_MODELCOMPILER_IOSTREAM_H

#include <QFile>

#include <assimp/IOStream.hpp>

#include <Volcano/ModelCompiler/Common.h>

VOLCANO_MODELCOMPILER_BEGIN

class IOStream: public Assimp::IOStream {
public:
    IOStream(const char* path, const char* mode);

public:
    size_t Read(void* buf, size_t size, size_t count) override;
    size_t Write(const void* buf, size_t size, size_t count) override;
    aiReturn Seek(size_t offset, aiOrigin origin) override;
    size_t Tell() const override;
    size_t FileSize() const override;
    void Flush() override;

private:
    QFile file_;
};

VOLCANO_MODELCOMPILER_END

#endif // VOLCANO_MODELCOMPILER_IOSTREAM_H
