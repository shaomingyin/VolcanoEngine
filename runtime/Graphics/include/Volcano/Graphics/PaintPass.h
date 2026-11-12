#ifndef VOLCANO_GRAPHICS_PAINTPASS_H
#define VOLCANO_GRAPHICS_PAINTPASS_H

#include <functional>
#include <map>
#include <string_view>
#include <tuple>
#include <vector>

#include <GL/gl.hpp>

#include <Volcano/World/Canvas.h>
#include <Volcano/Graphics/Common.h>
#include <Volcano/Graphics/Font.h>
#include <Volcano/Graphics/Target.h>
#include <Volcano/Graphics/Texture.h>

VOLCANO_GRAPHICS_BEGIN

class PaintPass final {
public:
    using FontResolver = std::function<const Font*(std::string_view name)>;
    using TextureResolver = std::function<const Texture*(const World::Canvas::Command& command)>;

    struct FontCacheStats {
        std::size_t atlasBytes = 0;
        std::size_t glyphs = 0;
        std::uint64_t hits = 0;
        std::uint64_t rasterizations = 0;
        std::uint64_t uploadedBytes = 0;
    };

    PaintPass();
    void render(const World::Canvas& commands, Target& target, bool clear);
    void setAtlasBudget(std::size_t bytes);
    std::size_t atlasBytes() const noexcept;
    FontCacheStats fontCacheStats() const noexcept;
    void prewarm(const Font& font, std::string_view text, unsigned int pixels);

    void setFontResolver(FontResolver resolver) {
        font_resolver_ = std::move(resolver);
    }

    void setTextureResolver(TextureResolver resolver) {
        texture_resolver_ = std::move(resolver);
    }

private:
    struct Vertex {
        float x, y, u, v, coverage;
    };
    struct Contour {
        std::vector<Eigen::Vector2f> points;
        bool closed = false;
    };
    struct Entry {
        Rectf uv;
        int left, top, width, height;
    };
    struct Page {
        gl::Texture2D texture;
        std::map<std::tuple<std::uint64_t, std::uint32_t, unsigned int>, Entry> entries;
        int x = 1;
        int y = 1;
        int row = 0;
        std::uint64_t used = 0;
    };
    struct GlyphImage {
        const gl::Texture2D* texture;
        Entry entry;
    };

    std::vector<Contour> flatten(const World::Canvas::Command& command) const;
    void curve(std::vector<Eigen::Vector2f>& points, const Eigen::Vector2f& a,
        const Eigen::Vector2f& b, const Eigen::Vector2f& c, const Eigen::Vector2f& d, int depth) const;
    void flushGlyphs();
    void draw(std::span<const Vertex> vertices, GLenum mode = GL_TRIANGLES);
    void paint(const World::Canvas::State& state, int textureMode);
    void shape(const World::Canvas::Command& command, bool stroke);
    void quad(const Rectf& rect, const Rectf& uv, const Eigen::Affine2f& transform);
    GlyphImage glyph(const Font& font, std::uint32_t index, unsigned int pixels);

    const Font* resolveFont(const World::Canvas::Command& command) const;
    const Texture* resolveTexture(const World::Canvas::Command& command) const;

private:
    gl::Program program_;
    gl::VertexArray vao_;
    gl::VertexBuffer buffer_;
    GLint viewport_, inner_, outer_, gradient_, start_, end_, radii_, inverse_, opacity_, mode_;
    int width_ = 0;
    int height_ = 0;
    std::vector<Page> pages_;
    std::vector<Vertex> glyph_vertices_;
    GLuint glyph_texture_ = 0;
    std::size_t budget_ = 4 * 1024 * 1024;
    std::uint64_t serial_ = 0;
    FontCacheStats stats_;
    FontResolver font_resolver_;
    TextureResolver texture_resolver_;
    static constexpr int PageSize = 1024;
};

VOLCANO_GRAPHICS_END

#endif
