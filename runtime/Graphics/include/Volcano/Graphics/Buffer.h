//
//
#ifndef VOLCANO_GRAPHICS_BUFFER_H
#define VOLCANO_GRAPHICS_BUFFER_H

#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

class Buffer {
public:
    enum class Type {
        Invalid = 0,
        Vertex,
        Index,
        Uniform,
        Storage,
        Indirect,
        TransferSource,
		TransferDestination,
    };

    enum class Usage {
        Static = 0,
        Dynamic,
        Stream,
	};

public:
	Buffer(size_t size, Type type, Usage usage = Usage::Static);
	virtual ~Buffer() = default;

public:
    Type type() const noexcept {
		return type_;
    }

	Usage usage() const noexcept {
        return usage_;
	}

    size_t size() const noexcept {
        return size_;
	}

	virtual void* map() = 0;
	virtual void unmap() = 0;
	virtual void update(const void* data, size_t size, size_t offset = 0) = 0;

private:
    Type type_;
    Usage usage_;
    size_t size_;
};

VOLCANO_GRAPHICS_END

#endif // VOLCANO_GRAPHICS_BUFFER_H
