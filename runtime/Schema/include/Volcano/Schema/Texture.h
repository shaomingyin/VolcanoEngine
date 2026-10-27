#ifndef VOLCANO_SCHEMA_TEXTURE_H
#define VOLCANO_SCHEMA_TEXTURE_H

#include <cstdint>

#include <Volcano/ByteArray.h>
#include <Volcano/Schema/Common.h>
#include <Volcano/Stream.h>

VOLCANO_SCHEMA_BEGIN

// Single-level RGBA8, bottom row first, with premultiplied alpha.
struct Texture final {
public:
    void validate() const;
    static Texture read(Stream& stream);
    void write(Stream& stream) const;

public:
    std::uint32_t width = 0;
    std::uint32_t height = 0;
    ByteArray pixels;
};

VOLCANO_SCHEMA_END

#endif // VOLCANO_SCHEMA_TEXTURE_H
