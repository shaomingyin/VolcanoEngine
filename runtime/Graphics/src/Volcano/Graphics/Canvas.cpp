#include <algorithm>
#include <cmath>
#include <numbers>
#include <stdexcept>

#include <Volcano/Graphics/Canvas.h>

VOLCANO_GRAPHICS_BEGIN

void Canvas::begin() {
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

void Canvas::setTransform(const Eigen::Affine2f& value) {
    VOLCANO_ASSERT(building_);
    if (!value.matrix().allFinite() || !std::isfinite(value.linear().determinant()) ||
        std::abs(value.linear().determinant()) < 1e-12f) {
        throw std::invalid_argument("VG transform must be finite and invertible");
    }
    state_.transform = value;
}

void Canvas::setOpacity(float value) {
    VOLCANO_ASSERT(building_);
    if (!(value >= 0 && value <= 1)) {
        throw std::invalid_argument("VG opacity must be in [0, 1]");
    }
    state_.opacity = value;
}

void Canvas::setClip(const Rectf& value) {
    VOLCANO_ASSERT(building_);
    if (!std::isfinite(value.left) || !std::isfinite(value.top) || !std::isfinite(value.width) ||
        !std::isfinite(value.height) || value.width < 0 || value.height < 0) {
        throw std::invalid_argument("Invalid VG clipping rectangle");
    }
    const Eigen::Vector2f a = state_.transform * value.topLeft();
    const Eigen::Vector2f b = state_.transform * value.topRight();
    const Eigen::Vector2f c = state_.transform * value.bottomLeft();
    const Eigen::Vector2f d = state_.transform * value.bottomRight();
    if (!a.allFinite() || !b.allFinite() || !c.allFinite() || !d.allFinite()) {
        throw std::invalid_argument("VG clipping transform overflow");
    }
    const float x = std::min({a.x(), b.x(), c.x(), d.x()});
    const float y = std::min({a.y(), b.y(), c.y(), d.y()});
    Rectf rect{x, y, std::max({a.x(), b.x(), c.x(), d.x()}) - x,
        std::max({a.y(), b.y(), c.y(), d.y()}) - y};
    state_.clip = state_.clip ? state_.clip->intersected(rect) : rect;
}

void Canvas::resetClip() {
    VOLCANO_ASSERT(building_);
    state_.clip.reset();
}

void Canvas::setColor(const Colorf& value) {
    VOLCANO_ASSERT(building_);
    state_.inner = value;
    state_.outer = value;
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
    if (!center.allFinite() || !(innerRadius >= 0 && outerRadius > innerRadius) || !std::isfinite(outerRadius)) {
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
    if (!(value > 0 && value <= 65536)) {
        throw std::invalid_argument("Invalid stroke width");
    }
    state_.strokeWidth = value;
}

void Canvas::setLineCap(LineCap value) {
    VOLCANO_ASSERT(building_);
    state_.cap = value;
}

void Canvas::setLineJoin(LineJoin value) {
    VOLCANO_ASSERT(building_);
    state_.join = value;
}

void Canvas::setMiterLimit(float value) {
    VOLCANO_ASSERT(building_);
    if (!(value >= 1 && value <= 65536)) {
        throw std::invalid_argument("Invalid miter limit");
    }
    state_.miterLimit = value;
}

void Canvas::setFont(const Font& font, float pixels) {
    VOLCANO_ASSERT(building_);
    if (!(pixels >= 1 && pixels <= 4096)) {
        throw std::invalid_argument("Invalid font size");
    }
    state_.font = &font;
    state_.pixels = pixels;
}

void Canvas::setTextAlign(TextAlign value) {
    VOLCANO_ASSERT(building_);
    state_.align = value;
}

void Canvas::beginPath() {
    VOLCANO_ASSERT(building_);
    path_.clear();
}

void Canvas::moveTo(float x, float y) {
    VOLCANO_ASSERT(building_);
    if (!std::isfinite(x) || !std::isfinite(y)) {
        throw std::invalid_argument("Non-finite VG coordinate");
    }
    path_.push_back({Verb::Move, {x, y}});
}

void Canvas::lineTo(float x, float y) {
    VOLCANO_ASSERT(building_ && !path_.empty());
    if (!std::isfinite(x) || !std::isfinite(y)) {
        throw std::invalid_argument("Non-finite VG coordinate");
    }
    path_.push_back({Verb::Line, {x, y}});
}

void Canvas::quadTo(float cx, float cy, float x, float y) {
    VOLCANO_ASSERT(building_ && !path_.empty());
    if (!Eigen::Vector4f(cx, cy, x, y).allFinite()) {
        throw std::invalid_argument("Non-finite VG curve");
    }
    path_.push_back({Verb::Quad, {x, y}, {cx, cy}});
}

void Canvas::bezierTo(float c1x, float c1y, float c2x, float c2y, float x, float y) {
    VOLCANO_ASSERT(building_ && !path_.empty());
    if (!Eigen::Vector4f(c1x, c1y, c2x, c2y).allFinite() || !Eigen::Vector2f(x, y).allFinite()) {
        throw std::invalid_argument("Non-finite VG curve");
    }
    path_.push_back({Verb::Cubic, {x, y}, {c1x, c1y}, {c2x, c2y}});
}

void Canvas::closePath() {
    VOLCANO_ASSERT(building_ && !path_.empty());
    path_.push_back({Verb::Close, {0, 0}});
}

void Canvas::rect(float x, float y, float width, float height) {
    moveTo(x, y);
    lineTo(x + width, y);
    lineTo(x + width, y + height);
    lineTo(x, y + height);
    closePath();
}

void Canvas::roundedRect(float x, float y, float width, float height, float radius) {
    if (!(width >= 0 && height >= 0 && radius >= 0)) {
        throw std::invalid_argument("Invalid rounded rectangle");
    }
    const float r = std::min({radius, width * 0.5f, height * 0.5f});
    constexpr float k = 0.55228475f;
    moveTo(x + r, y);
    lineTo(x + width - r, y);
    bezierTo(x + width - r + k*r, y, x + width, y + r - k*r, x + width, y + r);
    lineTo(x + width, y + height - r);
    bezierTo(x + width, y + height - r + k*r, x + width - r + k*r, y + height, x + width - r, y + height);
    lineTo(x + r, y + height);
    bezierTo(x + r - k*r, y + height, x, y + height - r + k*r, x, y + height - r);
    lineTo(x, y + r);
    bezierTo(x, y + r - k*r, x + r - k*r, y, x + r, y);
    closePath();
}

void Canvas::ellipse(float cx, float cy, float rx, float ry) {
    if (!(rx >= 0 && ry >= 0)) {
        throw std::invalid_argument("Invalid ellipse radius");
    }
    constexpr float k = 0.55228475f;
    moveTo(cx + rx, cy);
    bezierTo(cx + rx, cy + k*ry, cx + k*rx, cy + ry, cx, cy + ry);
    bezierTo(cx - k*rx, cy + ry, cx - rx, cy + k*ry, cx - rx, cy);
    bezierTo(cx - rx, cy - k*ry, cx - k*rx, cy - ry, cx, cy - ry);
    bezierTo(cx + k*rx, cy - ry, cx + rx, cy - k*ry, cx + rx, cy);
    closePath();
}

void Canvas::circle(float cx, float cy, float radius) {
    ellipse(cx, cy, radius, radius);
}

void Canvas::fill(FillRule rule) {
    VOLCANO_ASSERT(building_);
    Command command{Command::Type::Fill, state_, path_};
    command.rule = rule;
    commands_.push_back(std::move(command));
}

void Canvas::stroke() {
    VOLCANO_ASSERT(building_);
    commands_.push_back({Command::Type::Stroke, state_, path_});
}

void Canvas::image(const Texture& texture, const Rectf& destination) {
    VOLCANO_ASSERT(building_);
    if (!std::isfinite(destination.left) || !std::isfinite(destination.top) ||
        !std::isfinite(destination.width) || !std::isfinite(destination.height) ||
        destination.width < 0 || destination.height < 0) {
        throw std::invalid_argument("Invalid image rectangle");
    }
    Command command{Command::Type::Image, state_, {}};
    command.texture = &texture;
    command.rect = destination;
    commands_.push_back(std::move(command));
}

void Canvas::text(float x, float baseline, std::string_view text) {
    VOLCANO_ASSERT(building_ && state_.font != nullptr);
    if (!std::isfinite(x) || !std::isfinite(baseline)) {
        throw std::invalid_argument("Invalid text position");
    }
    Command command{Command::Type::Text, state_, {}};
    command.rect = {x, baseline, 0, 0};
    command.text = text;
    commands_.push_back(std::move(command));
}

std::vector<const Texture*> Canvas::sampledTextures() const {
    std::vector<const Texture*> result;
    for (const auto& command : commands_) {
        if (command.texture != nullptr) {
            result.push_back(command.texture);
        }
    }
    return result;
}

VOLCANO_GRAPHICS_END
