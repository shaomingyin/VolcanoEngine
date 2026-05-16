//
//
#include <Volcano/Graphics/Buffer.h>

VOLCANO_GRAPHICS_BEGIN

Buffer::Buffer(size_t size, Type type, Usage usage)
	: type_(type)
	, usage_(usage)
	, size_(size) {
}

VOLCANO_GRAPHICS_END
