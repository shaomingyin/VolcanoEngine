#include <algorithm>
#include <atomic>
#include <cmath>
#include <limits>
#include <stdexcept>
#include <string>

#include <ft2build.h>
#include FT_FREETYPE_H
#include <hb-ft.h>
#include <hb.h>

#include <Volcano/ScopeGuard.h>
#include <Volcano/Graphics/Font.h>

VOLCANO_GRAPHICS_BEGIN

Font::Font(std::span<const std::uint8_t> bytes) {
    if (bytes.empty() || bytes.size() > static_cast<std::size_t>(std::numeric_limits<FT_Long>::max())) {
        throw std::invalid_argument("Invalid font byte count");
    }
    bytes_.assign(bytes.begin(), bytes.end());
    auto rollback = scopeGuard([this] { release(); });
    if (FT_Init_FreeType(&library_) != 0 ||
        FT_New_Memory_Face(library_, bytes_.data(), static_cast<FT_Long>(bytes.size()), 0, &face_) != 0) {
        throw std::runtime_error("Cannot load font");
    }
    font_ = hb_ft_font_create_referenced(face_);
    if (font_ == hb_font_get_empty()) {
        throw std::runtime_error("Cannot create font shaper");
    }
    static std::atomic<std::uint64_t> next{1};
    id_ = next.fetch_add(1);
    rollback.dismiss();
}

Font::~Font() {
    release();
}

std::uint64_t Font::id() const noexcept {
    return id_;
}

std::size_t Font::byteSize() const noexcept {
    return bytes_.size();
}

std::uint64_t Font::revision() const noexcept {
    return revision_;
}

void Font::setFallbacks(std::span<const Font* const> fonts) {
    for (const auto* font : fonts) {
        if (font == nullptr || font == this) {
            throw std::invalid_argument("Font fallback must reference another font");
        }
    }
    fallbacks_.assign(fonts.begin(), fonts.end());
    ++revision_;
}

const std::vector<const Font*>& Font::fallbacks() const noexcept {
    return fallbacks_;
}

std::vector<Font::Glyph> Font::shape(std::string_view text, float pixels) const {
    setSize(pixels);
    if (text.size() > static_cast<std::size_t>(std::numeric_limits<int>::max())) {
        throw std::length_error("Text exceeds shaper length limit");
    }
    // Decode with HarfBuzz, then shape contiguous runs using a font that contains each character.
    auto* buffer = hb_buffer_create();
    auto cleanup = scopeGuard([buffer] { hb_buffer_destroy(buffer); });
    hb_buffer_add_utf8(buffer, text.data(), static_cast<int>(text.size()), 0, static_cast<int>(text.size()));
    unsigned int count = 0;
    const auto* info = hb_buffer_get_glyph_infos(buffer, &count);
    std::vector<std::uint32_t> codepoints;
    codepoints.reserve(count);
    for (unsigned int i = 0; i < count; ++i) {
        codepoints.push_back(info[i].codepoint);
    }
    auto choose = [this](std::uint32_t codepoint) {
        if (FT_Get_Char_Index(face_, codepoint) != 0) {
            return this;
        }
        for (const auto* font : fallbacks_) {
            if (FT_Get_Char_Index(font->face_, codepoint) != 0) {
                return font;
            }
        }
        return this;
    };
    std::vector<Glyph> result;
    float x = 0;
    float y = 0;
    for (std::size_t start = 0; start < codepoints.size();) {
        if (codepoints[start] == '\n') {
            x = 0;
            y += pixels * 1.2f;
            ++start;
            continue;
        }
        const auto* font = choose(codepoints[start]);
        std::size_t end = start + 1;
        while (end < codepoints.size() && codepoints[end] != '\n' && choose(codepoints[end]) == font) {
            ++end;
        }
        font->setSize(pixels);
        hb_buffer_clear_contents(buffer);
        hb_buffer_add_codepoints(buffer, codepoints.data() + start, static_cast<int>(end - start), 0,
            static_cast<int>(end - start));
        hb_buffer_guess_segment_properties(buffer);
        hb_shape(font->font_, buffer, nullptr, 0);
        const auto* glyphs = hb_buffer_get_glyph_infos(buffer, &count);
        const auto* positions = hb_buffer_get_glyph_positions(buffer, nullptr);
        for (unsigned int i = 0; i < count; ++i) {
            result.push_back({font, glyphs[i].codepoint, x + positions[i].x_offset / 64.0f,
                y - positions[i].y_offset / 64.0f});
            x += positions[i].x_advance / 64.0f;
            y -= positions[i].y_advance / 64.0f;
        }
        // A sentinel preserves the logical advance, including trailing whitespace, for measurement.
        result.push_back({nullptr, 0, x, y});
        start = end;
    }
    return result;
}

Sizef Font::measure(std::string_view text, float pixels) const {
    const auto glyphs = shape(text, pixels);
    float width = 0;
    for (const auto& glyph : glyphs) {
        if (glyph.font == nullptr) {
            width = std::max(width, glyph.x);
        }
    }
    const auto lines = 1 + std::count(text.begin(), text.end(), '\n');
    return {width, static_cast<float>(lines) * pixels * 1.2f};
}

Font::Bitmap Font::rasterize(std::uint32_t glyph, unsigned int pixels) const {
    setSize(static_cast<float>(pixels));
    if (FT_Load_Glyph(face_, glyph, FT_LOAD_DEFAULT) != 0 ||
        FT_Render_Glyph(face_->glyph, FT_RENDER_MODE_NORMAL) != 0) {
        throw std::runtime_error("Cannot rasterize glyph");
    }
    const auto& source = face_->glyph->bitmap;
    Bitmap result{static_cast<int>(source.width), static_cast<int>(source.rows),
        face_->glyph->bitmap_left, face_->glyph->bitmap_top, {}};
    result.pixels.resize(static_cast<std::size_t>(result.width) * result.height);
    for (int row = 0; row < result.height; ++row) {
        const auto* data = source.buffer + (source.pitch >= 0 ? row : result.height - 1 - row) * std::abs(source.pitch);
        for (int column = 0; column < result.width; ++column) {
            result.pixels[static_cast<std::size_t>(row) * result.width + column] = source.pixel_mode == FT_PIXEL_MODE_MONO
                ? ((data[column / 8] & (0x80 >> (column % 8))) != 0 ? 255 : 0) : data[column];
        }
    }
    return result;
}

void Font::release() noexcept {
    if (font_ != nullptr) {
        hb_font_destroy(font_);
    }
    if (face_ != nullptr) {
        FT_Done_Face(face_);
    }
    if (library_ != nullptr) {
        FT_Done_FreeType(library_);
    }
}

void Font::setSize(float pixels) const {
    if (!std::isfinite(pixels) || pixels < 1 || pixels > 4096) {
        throw std::invalid_argument("Font pixel size must be in [1, 4096]");
    }
    if (FT_Set_Char_Size(face_, 0, static_cast<FT_F26Dot6>(pixels * 64), 72, 72) != 0) {
        throw std::runtime_error("Cannot set font size");
    }
    hb_ft_font_changed(font_);
}

VOLCANO_GRAPHICS_END
