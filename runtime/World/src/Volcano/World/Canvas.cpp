#include <algorithm>
#include <cmath>
#include <numbers>
#include <stdexcept>
#include <utility>

#include <Volcano/World/Common.h>
#include <Volcano/World/Canvas.h>

VOLCANO_WORLD_BEGIN

Canvas::Canvas(Size size) noexcept
    : Canvas(size, Texture::Kind::Canvas) {}

Canvas::Canvas(Size size, Texture::Kind kind) noexcept
    : Texture(kind), size_(size) {}

void Canvas::begin() noexcept {
    state_ = {};
    stack_.clear();
    path_.clear();
    commands_.clear();
    building_ = true;
    ready_ = false;
}

void Canvas::end() {
    VOLCANO_ASSERT(building_);
    VOLCANO_ASSERT(stack_.empty());
    building_ = false;
    ready_ = true;
    ++revision_;
}

void Canvas::save() {
    VOLCANO_ASSERT(building_);
    stack_.push_back(state_);
}

void Canvas::restore() {
    VOLCANO_ASSERT(building_ && !stack_.empty());
    state_ = stack_.back();
    stack_.pop_back();
}

void Canvas::setTransform(const Eigen::Affine2f& transform) {
    VOLCANO_ASSERT(building_);
    if (!transform.matrix().allFinite() || !std::isfinite(transform.linear().determinant()) ||
        std::abs(transform.linear().determinant()) < 1e-12f) {
        throw std::invalid_argument("Canvas transform must be finite and invertible");
    }
    state_.transform = transform;
}

void Canvas::setOpacity(float value) {
    VOLCANO_ASSERT(building_);
    if (!std::isfinite(value) || value < 0.0f || value > 1.0f) {
        throw std::invalid_argument("Canvas opacity must be in [0, 1]");
    }
    state_.opacity = value;
}

void Canvas::setClip(const Rectf& rect) {
    VOLCANO_ASSERT(building_);
    if (!std::isfinite(rect.left) || !std::isfinite(rect.top) || !std::isfinite(rect.width) ||
        !std::isfinite(rect.height) || rect.width < 0.0f || rect.height < 0.0f) {
        throw std::invalid_argument("Canvas clipping rectangle must be finite with nonnegative dimensions");
    }
    const Eigen::Vector2f a = state_.transform * rect.topLeft();
    const Eigen::Vector2f b = state_.transform * rect.topRight();
    const Eigen::Vector2f c = state_.transform * rect.bottomLeft();
    const Eigen::Vector2f d = state_.transform * rect.bottomRight();
    if (!a.allFinite() || !b.allFinite() || !c.allFinite() || !d.allFinite()) {
        throw std::invalid_argument("Canvas clipping transform overflow");
    }
    const float x = std::min({a.x(), b.x(), c.x(), d.x()});
    const float y = std::min({a.y(), b.y(), c.y(), d.y()});
    const Rectf transformedRect{
        x,
        y,
        std::max({a.x(), b.x(), c.x(), d.x()}) - x,
        std::max({a.y(), b.y(), c.y(), d.y()}) - y
    };
    state_.clip = state_.clip ? state_.clip->intersected(transformedRect) : transformedRect;
}

void Canvas::resetClip() noexcept {
    VOLCANO_ASSERT(building_);
    state_.clip.reset();
}

void Canvas::setColor(const Colorf& color) noexcept {
    VOLCANO_ASSERT(building_);
    state_.inner = color;
    state_.outer = color;
    state_.gradient = 0;
}

void Canvas::setLinearGradient(const Eigen::Vector2f& start, const Eigen::Vector2f& end,
    const Colorf& inner, const Colorf& outer) {
    VOLCANO_ASSERT(building_);
    if (!start.allFinite() || !end.allFinite() || (end - start).squaredNorm() < 1e-12f) {
        throw std::invalid_argument("Invalid linear gradient");
    }
    state_.start = start;
    state_.end = end;
    state_.inner = inner;
    state_.outer = outer;
    state_.gradient = 1;
}

void Canvas::setRadialGradient(const Eigen::Vector2f& center, float innerRadius, float outerRadius,
    const Colorf& inner, const Colorf& outer) {
    VOLCANO_ASSERT(building_);
    if (!center.allFinite() || !std::isfinite(innerRadius) || !std::isfinite(outerRadius) ||
        innerRadius < 0.0f || outerRadius <= innerRadius) {
        throw std::invalid_argument("Invalid radial gradient");
    }
    state_.start = center;
    state_.innerRadius = innerRadius;
    state_.outerRadius = outerRadius;
    state_.inner = inner;
    state_.outer = outer;
    state_.gradient = 2;
}

void Canvas::setStrokeWidth(float value) {
    VOLCANO_ASSERT(building_);
    if (!std::isfinite(value) || value <= 0.0f) {
        throw std::invalid_argument("Canvas stroke width must be positive");
    }
    state_.strokeWidth = value;
}

void Canvas::setLineCap(LineCap value) noexcept {
    VOLCANO_ASSERT(building_);
    state_.cap = value;
}

void Canvas::setLineJoin(LineJoin value) noexcept {
    VOLCANO_ASSERT(building_);
    state_.join = value;
}

void Canvas::setMiterLimit(float value) {
    VOLCANO_ASSERT(building_);
    if (!std::isfinite(value) || value < 1.0f) {
        throw std::invalid_argument("Canvas miter limit must be at least 1");
    }
    state_.miterLimit = value;
}

void Canvas::setFont(std::string font, float pixels) {
    VOLCANO_ASSERT(building_);
    if (!std::isfinite(pixels) || pixels <= 0.0f) {
        throw std::invalid_argument("Canvas font size must be positive");
    }
    state_.font = std::move(font);
    state_.pixels = pixels;
}

void Canvas::setTextAlign(TextAlign value) noexcept {
    VOLCANO_ASSERT(building_);
    state_.align = value;
}

void Canvas::beginPath() noexcept {
    VOLCANO_ASSERT(building_);
    path_.clear();
}

void Canvas::moveTo(float x, float y) {
    VOLCANO_ASSERT(building_);
    if (!std::isfinite(x) || !std::isfinite(y)) {
        throw std::invalid_argument("Canvas path coordinate must be finite");
    }
    path_.push_back({Verb::Move, {x, y}});
}

void Canvas::lineTo(float x, float y) {
    VOLCANO_ASSERT(building_);
    if (!std::isfinite(x) || !std::isfinite(y)) {
        throw std::invalid_argument("Canvas path coordinate must be finite");
    }
    path_.push_back({Verb::Line, {x, y}});
}

void Canvas::quadTo(float cx, float cy, float x, float y) {
    VOLCANO_ASSERT(building_);
    if (!std::isfinite(cx) || !std::isfinite(cy) || !std::isfinite(x) || !std::isfinite(y)) {
        throw std::invalid_argument("Canvas quad coordinates must be finite");
    }
    path_.push_back({Verb::Quad, {x, y}, {cx, cy}});
}

void Canvas::bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y) {
    VOLCANO_ASSERT(building_);
    if (!std::isfinite(c1x) || !std::isfinite(c1y) || !std::isfinite(c2x) || !std::isfinite(c2y) ||
        !std::isfinite(x) || !std::isfinite(y)) {
        throw std::invalid_argument("Canvas cubic coordinates must be finite");
    }
    path_.push_back({Verb::Cubic, {x, y}, {c1x, c1y}, {c2x, c2y}});
}

void Canvas::closePath() {
    VOLCANO_ASSERT(building_);
    path_.push_back({Verb::Close, {0.0f, 0.0f}});
}

void Canvas::rect(const Rectf& rect) {
    beginPath();
    moveTo(rect.left, rect.top);
    lineTo(rect.right(), rect.top);
    lineTo(rect.right(), rect.bottom());
    lineTo(rect.left, rect.bottom());
    closePath();
}

void Canvas::rect(float x, float y, float width, float height) {
    rect(Rectf{x, y, width, height});
}

void Canvas::roundedRect(const Rectf& rect, float radius) {
    if (radius <= 0.0f) {
        this->rect(rect);
        return;
    }
    beginPath();
    const float r = std::min({radius, rect.width * 0.5f, rect.height * 0.5f});
    moveTo(rect.left + r, rect.top);
    lineTo(rect.right() - r, rect.top);
    quadTo(rect.right(), rect.top, rect.right(), rect.top + r);
    lineTo(rect.right(), rect.bottom() - r);
    quadTo(rect.right(), rect.bottom(), rect.right() - r, rect.bottom());
    lineTo(rect.left + r, rect.bottom());
    quadTo(rect.left, rect.bottom(), rect.left, rect.bottom() - r);
    lineTo(rect.left, rect.top + r);
    quadTo(rect.left, rect.top, rect.left + r, rect.top);
    closePath();
}

void Canvas::roundedRect(float x, float y, float width, float height, float radius) {
    roundedRect(Rectf{x, y, width, height}, radius);
}

void Canvas::ellipse(float cx, float cy, float rx, float ry) {
    if (!std::isfinite(cx) || !std::isfinite(cy) || !std::isfinite(rx) || !std::isfinite(ry) ||
        rx <= 0.0f || ry <= 0.0f) {
        throw std::invalid_argument("Canvas ellipse parameters must be finite with positive radii");
    }
    beginPath();
    const float kx = 0.5522847498f * rx;
    const float ky = 0.5522847498f * ry;
    moveTo(cx + rx, cy);
    bezierTo(cx + rx, cy + ky, cx + kx, cy + ry, cx, cy + ry);
    bezierTo(cx - kx, cy + ry, cx - rx, cy + ky, cx - rx, cy);
    bezierTo(cx - rx, cy - ky, cx - kx, cy - ry, cx, cy - ry);
    bezierTo(cx + kx, cy - ry, cx + rx, cy - ky, cx + rx, cy);
    closePath();
}

void Canvas::circle(const Eigen::Vector2f& center, float radius) {
    ellipse(center.x(), center.y(), radius, radius);
}

void Canvas::circle(float cx, float cy, float radius) {
    ellipse(cx, cy, radius, radius);
}

void Canvas::fill(FillRule rule) {
    VOLCANO_ASSERT(building_);
    if (!path_.empty()) {
        commands_.push_back(Command{
            .type = Command::Type::Fill,
            .state = state_,
            .path = path_,
            .rule = rule
        });
    }
}

void Canvas::stroke() {
    VOLCANO_ASSERT(building_);
    if (!path_.empty()) {
        commands_.push_back(Command{
            .type = Command::Type::Stroke,
            .state = state_,
            .path = path_
        });
    }
}

void Canvas::stroke(float width) {
    setStrokeWidth(width);
    stroke();
}

void Canvas::image(std::string texture, const Rectf& destination) {
    VOLCANO_ASSERT(building_);
    if (texture.empty()) {
        throw std::invalid_argument("Canvas image texture name cannot be empty");
    }
    if (!std::isfinite(destination.left) || !std::isfinite(destination.top) ||
        !std::isfinite(destination.width) || !std::isfinite(destination.height) ||
        destination.width <= 0.0f || destination.height <= 0.0f) {
        throw std::invalid_argument("Canvas destination rectangle must be finite with positive dimensions");
    }
    commands_.push_back(Command{
        .type = Command::Type::Image,
        .state = state_,
        .texture = std::move(texture),
        .destination = destination
    });
}

void Canvas::image(std::shared_ptr<Texture> texture, const Rectf& destination) {
    VOLCANO_ASSERT(building_);
    if (!texture) {
        throw std::invalid_argument("Canvas image texture reference cannot be null");
    }
    if (!std::isfinite(destination.left) || !std::isfinite(destination.top) ||
        !std::isfinite(destination.width) || !std::isfinite(destination.height) ||
        destination.width <= 0.0f || destination.height <= 0.0f) {
        throw std::invalid_argument("Canvas destination rectangle must be finite with positive dimensions");
    }
    commands_.push_back(Command{
        .type = Command::Type::Image,
        .state = state_,
        .textureRef = std::move(texture),
        .destination = destination
    });
}

void Canvas::image(const Texture& texture, const Rectf& destination) {
    std::shared_ptr<Texture> nonOwning(const_cast<Texture*>(&texture), [](Texture*) {});
    image(std::move(nonOwning), destination);
}

void Canvas::text(std::string font, float pixels, float x, float baseline, std::string_view text,
    TextAlign align) {
    VOLCANO_ASSERT(building_);
    if (!std::isfinite(pixels) || pixels <= 0.0f || !std::isfinite(x) || !std::isfinite(baseline)) {
        throw std::invalid_argument("Canvas text metric parameters must be finite with positive font size");
    }
    State textState = state_;
    textState.font = font;
    textState.pixels = pixels;
    textState.align = align;

    commands_.push_back(Command{
        .type = Command::Type::Text,
        .state = std::move(textState),
        .destination = Rectf{x, baseline, 0.0f, 0.0f},
        .font = std::move(font),
        .pixels = pixels,
        .x = x,
        .baseline = baseline,
        .text = std::string(text)
    });
}

void Canvas::text(float x, float baseline, std::string_view text) {
    this->text(state_.font, state_.pixels, x, baseline, text, state_.align);
}

std::vector<std::shared_ptr<Texture>> Canvas::sampledTextures() const {
    std::vector<std::shared_ptr<Texture>> result;
    for (const auto& cmd : commands_) {
        if (cmd.type == Command::Type::Image && cmd.textureRef) {
            result.push_back(cmd.textureRef);
        }
    }
    return result;
}

void Canvas::collectResources(std::vector<std::filesystem::path>& fonts,
                              std::vector<std::string>& images) const {
    for (const auto& cmd : commands_) {
        if (cmd.type == Command::Type::Text && !cmd.font.empty()) {
            fonts.emplace_back(cmd.font);
        } else if (cmd.type == Command::Type::Image && !cmd.texture.empty()) {
            images.push_back(cmd.texture);
        }
    }
}

VOLCANO_WORLD_END
