#ifndef VOLCANO_GRAPHICS_FONT_H
#define VOLCANO_GRAPHICS_FONT_H

#include <cstdint>
#include <span>
#include <string_view>
#include <vector>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <hb.h>

#include <Volcano/Size.h>
#include <Volcano/Graphics/Common.h>

VOLCANO_GRAPHICS_BEGIN

// Owns font bytes and CPU font processing; no graphics context is required.
class Font final {
public:
    struct Glyph {
        const Font* font;
        std::uint32_t index;
        float x;
        float y;
    };

    struct Bitmap {
        int width;
        int height;
        int left;
        int top;
        std::vector<std::uint8_t> pixels;
    };

    explicit Font(std::span<const std::uint8_t> bytes);
    ~Font();
    Font(const Font&) = delete;
    Font& operator=(const Font&) = delete;

    void setFallbacks(std::span<const Font* const> fonts);
    const std::vector<const Font*>& fallbacks() const noexcept;
    std::vector<Glyph> shape(std::string_view text, float pixels) const;
    Sizef measure(std::string_view text, float pixels) const;
    Bitmap rasterize(std::uint32_t glyph, unsigned int pixels) const;
    std::uint64_t id() const noexcept;
    std::size_t byteSize() const noexcept;
    std::uint64_t revision() const noexcept;

private:
    void setSize(float pixels) const;
    void release() noexcept;

private:
    std::vector<std::uint8_t> bytes_;
    FT_Library library_ = nullptr;
    FT_Face face_ = nullptr;
    hb_font_t* font_ = nullptr;
    std::vector<const Font*> fallbacks_;
    std::uint64_t id_ = 0;
    std::uint64_t revision_ = 1;
};

VOLCANO_GRAPHICS_END

#endif
