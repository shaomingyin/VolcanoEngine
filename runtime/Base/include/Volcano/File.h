//
//
#ifndef VOLCANO_FILE_H
#define VOLCANO_FILE_H

#include <Volcano/Stream.h>

#include <Volcano/Common.h>

VOLCANO_BEGIN

// Implementations retain their storage so handles remain valid after their filesystem is destroyed.
class File : public Stream {
public:
    bool isSequential() const override {
        return false;
    }

    void seek(std::size_t position) override = 0;
    std::size_t tell() const override = 0;
    std::size_t size() override = 0;

    using Stream::read;

    // Preserve whole-file reads independently of the current position.
    ByteArray read() {
        seek(0);
        ByteArray result(size());
        readExact(result);
        return result;
    }
};

VOLCANO_END

#endif // VOLCANO_FILE_H
