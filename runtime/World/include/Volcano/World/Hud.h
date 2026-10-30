#ifndef VOLCANO_WORLD_HUD_H
#define VOLCANO_WORLD_HUD_H

#include <string>
#include <cmath>
#include <stdexcept>
#include <utility>

#include <Volcano/Rect.h>
#include <Volcano/World/Common.h>

VOLCANO_WORLD_BEGIN

class Hud final {
public:
    const std::string& texture() const noexcept {
        return texture_;
    }
    void setTexture(std::string value) {
        texture_ = std::move(value);
    }
    const Rectf& rect() const noexcept {
        return rect_;
    }
    void setRect(const Rectf& value) {
        if (!std::isfinite(value.left) || !std::isfinite(value.top) || !std::isfinite(value.width) ||
            !std::isfinite(value.height) || value.width <= 0 || value.height <= 0) {
            throw std::invalid_argument("HUD rectangle must be finite with positive dimensions");
        }
        rect_ = value;
    }
    int order() const noexcept {
        return order_;
    }
    void setOrder(int value) noexcept {
        order_ = value;
    }

private:
    std::string texture_;
    Rectf rect_{};
    int order_ = 0;
};

VOLCANO_WORLD_END

#endif
