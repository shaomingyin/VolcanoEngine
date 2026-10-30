#ifndef VOLCANO_GRAPHICS_CANVAS_H
#define VOLCANO_GRAPHICS_CANVAS_H

#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <Eigen/Geometry>

#include <Volcano/Color.h>
#include <Volcano/Rect.h>
#include <Volcano/Graphics/Font.h>
#include <Volcano/Graphics/Texture.h>

VOLCANO_GRAPHICS_BEGIN

// Owns commands and text; images and fonts must outlive every submitted use.
class Canvas final {
public:
    enum class FillRule { NonZero, EvenOdd };
    enum class LineCap { Butt, Square, Round };
    enum class LineJoin { Miter, Bevel, Round };
    enum class TextAlign { Left, Center, Right };

    void begin();
    void end();
    bool ready() const noexcept {
        return ready_;
    }
    void save();
    void restore();
    void setTransform(const Eigen::Affine2f& transform);
    void setOpacity(float value);
    void setClip(const Rectf& rect);
    void resetClip();
    void setColor(const Colorf& color);
    void setLinearGradient(const Eigen::Vector2f& start, const Eigen::Vector2f& end,
        const Colorf& inner, const Colorf& outer);
    void setRadialGradient(const Eigen::Vector2f& center, float innerRadius, float outerRadius,
        const Colorf& inner, const Colorf& outer);
    void setStrokeWidth(float value);
    void setLineCap(LineCap value);
    void setLineJoin(LineJoin value);
    void setMiterLimit(float value);
    void setFont(const Font& font, float pixels);
    void setTextAlign(TextAlign value);
    void beginPath();
    void moveTo(float x, float y);
    void lineTo(float x, float y);
    void quadTo(float cx, float cy, float x, float y);
    void bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y);
    void closePath();
    void rect(float x, float y, float width, float height);
    void roundedRect(float x, float y, float width, float height, float radius);
    void ellipse(float cx, float cy, float rx, float ry);
    void circle(float cx, float cy, float radius);
    void fill(FillRule rule = FillRule::NonZero);
    void stroke();
    void image(const Texture& texture, const Rectf& destination);
    void text(float x, float baseline, std::string_view text);
    std::vector<const Texture*> sampledTextures() const;

private:
    friend class PaintPass;
    enum class Verb { Move, Line, Quad, Cubic, Close };
    struct Segment {
        Verb verb;
        Eigen::Vector2f point;
        Eigen::Vector2f control1{};
        Eigen::Vector2f control2{};
    };
    struct State {
        Eigen::Affine2f transform = Eigen::Affine2f::Identity();
        std::optional<Rectf> clip;
        Colorf inner = Colorf::White;
        Colorf outer = Colorf::White;
        Eigen::Vector2f start = Eigen::Vector2f::Zero();
        Eigen::Vector2f end = Eigen::Vector2f::UnitX();
        int gradient = 0;
        float innerRadius = 0;
        float outerRadius = 1;
        float opacity = 1;
        float strokeWidth = 1;
        float miterLimit = 4;
        LineCap cap = LineCap::Butt;
        LineJoin join = LineJoin::Miter;
        const Font* font = nullptr;
        float pixels = 16;
        TextAlign align = TextAlign::Left;
    };
    struct Command {
        enum class Type { Fill, Stroke, Image, Text };
        Type type;
        State state;
        std::vector<Segment> path;
        FillRule rule = FillRule::NonZero;
        const Texture* texture = nullptr;
        Rectf rect{};
        std::string text;
    };

private:
    State state_;
    std::vector<State> stack_;
    std::vector<Segment> path_;
    std::vector<Command> commands_;
    bool building_ = false;
    bool ready_ = false;
};

VOLCANO_GRAPHICS_END

#endif
