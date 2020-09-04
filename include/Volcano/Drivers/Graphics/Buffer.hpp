//
//
#ifndef VOLCANO_DRIVERS_GRAPHICS_BUFFER_HPP
#define VOLCANO_DRIVERS_GRAPHICS_BUFFER_HPP

#include <QIODevice>

#include <Volcano/Drivers/Graphics/Common.hpp>

VOLCANO_DRIVERS_GRAPHICS_BEGIN

class VOLCANO_API Buffer: public QIODevice
{
    Q_OBJECT

public:
    Buffer(QObject *parent = nullptr);
    ~Buffer(void);

public:
    virtual void *map(void) = 0;
    virtual void unmap(void) = 0;
};

VOLCANO_DRIVERS_GRAPHICS_END

#endif // VOLCANO_DRIVERS_GRAPHICS_BUFFER_HPP
