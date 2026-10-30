#include <cstddef>
#include <cstdint>
#include <span>
#include <stdexcept>

#include "Format.h"

#include <Volcano/Schema/Texture.h>

VOLCANO_SCHEMA_BEGIN

void Texture::validate() const {
    if (width == 0 || height == 0 ||
        pixels.size() != detail::checkedProduct(detail::checkedProduct(width, height), 4)) {
        throw std::invalid_argument("Texture requires positive dimensions and width * height * 4 bytes");
    }

    for (std::size_t offset = 0; offset < pixels.size(); offset += 4) {
        if (pixels[offset] > pixels[offset + 3] || pixels[offset + 1] > pixels[offset + 3] ||
            pixels[offset + 2] > pixels[offset + 3]) {
            throw std::invalid_argument("Texture RGB channels must be premultiplied by alpha");
        }
    }
}

Texture Texture::read(Stream& stream) {
    const auto bytes = stream.readAll();
    std::span<const std::uint8_t> input(bytes);
    detail::requireBinaryHeader(input, "VETX");
    Texture result;
    result.width = detail::readU32(input);
    result.height = detail::readU32(input);
    const auto payloadSize = detail::checkedProduct(detail::checkedProduct(result.width, result.height), 4);
    if (input.size() != payloadSize) {
        throw std::invalid_argument("Texture payload length does not match its dimensions");
    }

    result.pixels.assign(input.begin(), input.end());
    result.validate();
    return result;
}

void Texture::write(Stream& stream) const {
    validate();
    ByteArray header{'V', 'E', 'T', 'X'};
    detail::appendU32(header, 1);
    detail::appendU32(header, width);
    detail::appendU32(header, height);
    stream.writeAll(header);
    stream.writeAll(pixels);
}

VOLCANO_SCHEMA_END
