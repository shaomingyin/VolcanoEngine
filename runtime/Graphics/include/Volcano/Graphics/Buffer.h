//
//
#ifndef VOLCANO_GRAPHICS_BUFFER_H
#define VOLCANO_GRAPHICS_BUFFER_H

#include <QIODevice>

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Buffer: public QIODevice {
    Q_OBJECT

public:
    using MapMode = OpenMode;

public:
    Buffer() = default;
    virtual ~Buffer() = default;

public:
    bool isSequential() const override;
    virtual bool bind() = 0;
    virtual void *map(MapMode mode = ReadWrite) = 0;
    virtual void *mapRange(int offset, int count, MapMode mode = ReadWrite) = 0;
    virtual void unmap() = 0;

protected:
    qint64 readData(char* data, qint64 max_size) override;
    qint64 writeData(const char* data, qint64 max_size) override;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_BUFFER_H
