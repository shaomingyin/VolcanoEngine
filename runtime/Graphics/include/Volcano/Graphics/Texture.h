//
//
#ifndef VOLCANO_GRAPHICS_TEXTURE_H
#define VOLCANO_GRAPHICS_TEXTURE_H

#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Target.h>

VOLCANO_GRAPHICS_BEGIN

class Texture: public Target {
public:
    enum class Type {
        Texture1D,
        Texture2D,
        Texture3D,
        Cube
    };

    enum class Format {
        RGBA8_UNorm,
        RGBA8_SRGB,
        BGRA8_UNorm,
        BGRA8_SRGB,
        R16_Float,
        RG16_Float,
        RGBA16_Float,
        R32_Float,
        D24S8,
        D32_Float
    };

    enum class Usage : uint32_t {
        Sampled = 1 << 0, // shader read
        Storage = 1 << 1, // shader read/write
        RenderTarget = 1 << 2,
        DepthStencil = 1 << 3,
        TransferSrc = 1 << 4,
        TransferDst = 1 << 5,
        Present = 1 << 6
    };

public:
	Texture(Type type, Format format, Usage usage, uint32_t width, uint32_t height, uint32_t depth = 0);
	virtual ~Texture() = default;

public:
    uint32_t depth() const noexcept {
		VOLCANO_ASSERT(type_ >= Type::Texture3D);
        return depth_;
    }

    Type type() const noexcept {
        return type_;
    }

    Format format() const noexcept {
        return format_;
    }

	Usage usage() const noexcept {
        return usage_;
    }

	virtual void update(const void* data, size_t size) = 0;

private:
	Type type_;
	Format format_;
	Usage usage_;
	uint32_t depth_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_TEXTURE_H
