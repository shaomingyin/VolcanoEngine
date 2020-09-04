//
//
#include <Volcano/Drivers/Graphics/Buffer.hpp>

VOLCANO_DRIVERS_GRAPHICS_BEGIN

Buffer::Buffer(QObject *parent):
    QIODevice(parent)
{
}

Buffer::~Buffer(void)
{
}

VOLCANO_DRIVERS_GRAPHICS_END
