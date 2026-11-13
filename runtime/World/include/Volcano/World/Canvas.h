#ifndef VOLCANO_WORLD_CANVAS_H
#define VOLCANO_WORLD_CANVAS_H

#include <algorithm>
#include <cmath>
#include <memory>
#include <optional>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

#include <Eigen/Geometry>

#include <Volcano/Color.h>
#include <Volcano/Rect.h>
#include <Volcano/Size.h>
#include <Volcano/World/Common.h>
#include <Volcano/World/Texture.h>

VOLCANO_WORLD_BEGIN

class Canvas {
public:
    enum class FillRule {
        NonZero,
        EvenOdd
    };

    enum class LineCap {
        Butt,
        Square,
        Round
    };

    enum class LineJoin {
        Miter,
        Bevel,
        Round
    };

    enum class TextAlign {
        Left,
        Center,
        Right
    };

    enum class Verb {
        Move,
        Line,
        Quad,
        Cubic,
        Close
    };

    struct Segment final {
        Verb verb = Verb::Move;
        Eigen::Vector2f point = Eigen::Vector2f::Zero();
        Eigen::Vector2f control1 = Eigen::Vector2f::Zero();
        Eigen::Vector2f control2 = Eigen::Vector2f::Zero();
    };

    struct State final {
        Eigen::Affine2f transform = Eigen::Affine2f::Identity();
        std::optional<Rectf> clip;
        Colorf inner = Colorf::White;
        Colorf outer = Colorf::White;
        Eigen::Vector2f start = Eigen::Vector2f::Zero();
        Eigen::Vector2f end = Eigen::Vector2f::UnitX();
        int gradient = 0;
        float innerRadius = 0.0f;
        float outerRadius = 1.0f;
        float opacity = 1.0f;
        float strokeWidth = 1.0f;
        float miterLimit = 4.0f;
        LineCap cap = LineCap::Butt;
        LineJoin join = LineJoin::Miter;
        std::string font;
        float pixels = 16.0f;
        TextAlign align = TextAlign::Left;
    };

    struct Command final {
        enum class Type {
            Fill,
            Stroke,
            Image,
            Text
        };

        Type type = Type::Fill;
        State state;
        std::vector<Segment> path;
        FillRule rule = FillRule::NonZero;
        std::string texture;
        std::shared_ptr<Texture> textureRef;
        Rectf destination{};
        std::string font;
        float pixels = 16.0f;
        float x = 0.0f;
        float baseline = 0.0f;
        std::string text;
    };

public:
    explicit Canvas(Size size = Size{}) noexcept;
    virtual ~Canvas() = default;

    Size size() const noexcept {
        return size_;
    }

    void setSize(Size size) noexcept {
        size_ = size;
    }

    void begin() noexcept;
    void end();

    bool ready() const noexcept {
        return ready_;
    }

    bool building() const noexcept {
        return building_;
    }

    bool empty() const noexcept {
        return commands_.empty();
    }

    void clear() noexcept {
        begin();
    }

    void save();
    void restore();

    void setTransform(const Eigen::Affine2f& transform);
    void setOpacity(float value);
    void setClip(const Rectf& rect);
    void resetClip() noexcept;

    void setColor(const Colorf& color) noexcept;
    void setLinearGradient(const Eigen::Vector2f& start, const Eigen::Vector2f& end,
        const Colorf& inner, const Colorf& outer);
    void setRadialGradient(const Eigen::Vector2f& center, float innerRadius, float outerRadius,
        const Colorf& inner, const Colorf& outer);

    void setStrokeWidth(float value);
    void setLineCap(LineCap value) noexcept;
    void setLineJoin(LineJoin value) noexcept;
    void setMiterLimit(float value);

    void setFont(std::string font, float pixels);
    void setTextAlign(TextAlign value) noexcept;

    void beginPath() noexcept;
    void moveTo(float x, float y);
    void lineTo(float x, float y);
    void quadTo(float cx, float cy, float x, float y);
    void bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y);
    void closePath();

    void rect(const Rectf& rect);
    void rect(float x, float y, float width, float height);
    void roundedRect(const Rectf& rect, float radius);
    void roundedRect(float x, float y, float width, float height, float radius);
    void ellipse(float cx, float cy, float rx, float ry);
    void circle(const Eigen::Vector2f& center, float radius);
    void circle(float cx, float cy, float radius);

    void fill(FillRule rule = FillRule::NonZero);
    void stroke();
    void stroke(float width);

    void image(std::string texture, const Rectf& destination);
    void image(std::shared_ptr<Texture> texture, const Rectf& destination);
    void image(const Texture& texture, const Rectf& destination);
    void text(std::string font, float pixels, float x, float baseline, std::string_view text,
        TextAlign align = TextAlign::Left);
    void text(float x, float baseline, std::string_view text);

    const std::vector<Command>& commands() const noexcept {
        return commands_;
    }

    std::vector<std::shared_ptr<Texture>> sampledTextures() const;

private:
    Size size_;
    State state_;
    std::vector<State> stack_;
    std::vector<Segment> path_;
    std::vector<Command> commands_;
    bool building_ = false;
    bool ready_ = false;
};

VOLCANO_WORLD_END

#endif // VOLCANO_WORLD_CANVAS_H
