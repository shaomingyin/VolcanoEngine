//
//
#include <Volcano/Graphics/Texture.h>

VOLCANO_GRAPHICS_BEGIN

Texture::Texture(Type type, Format format, Usage usage, uint32_t width, uint32_t height, uint32_t depth)
	: Target(width, height)
	, type_(type)
	, format_(format)
	, usage_(usage)
	, depth_(depth) {
}

VOLCANO_GRAPHICS_END
